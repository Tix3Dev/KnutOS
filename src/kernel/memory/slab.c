/*
	BSD 3-Clause License

	Copyright (c) 2022, V01D
	All rights reserved.

	https://github.com/V01D-NULL/MoonOS/blob/c82b2a4d179077dfc0f7210902abd879f80377df/kernel/mm/slab.c
*/

/*
	The first version was developed by Yves Vollmeier and Tim Thompson (in collaboration)
	and the code can be seen here:
	https://github.com/V01D-NULL/slab_allocator
	This version was first developed by Tim Thompson and modified by
	Yves Vollmeier. Copyright header at the top is just for sanity.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <boot/stivale2.h>
#include <boot/stivale2_boot.h>
#include <memory/mem.h>
#include <memory/pmm.h>
#include <memory/slab.h>
#include <libk/debug/debug.h>
#include <libk/log/log.h>
#include <libk/math/math.h>
#include <libk/stdio/stdio.h>

/*  Explanation of the method used here for slab allocations:
    Instead of a linked list which would normally be used, this allocator
    is based on an array. It holds 10 slabs. The smallest size is 2^1
    and the biggest is 2^9.
    Each slab contains an array of objects.
    Each object is either
    free - holding an address of free memory
    or
    used - holding NULL.

    Now, when allocating memory, we search for a free object in the slab
    of the machting size (parameter). If we find one, we set it to used
    and return it's pointer. So, that it's memory can't be used anymore.

    UNTIL we free the memory (by passing it's pointer) in slab_free.
    For that we just search for a used object and then set it to the
    passed pointer. The memory which the pointer points to, can now be
    overwritten i.e. used again once somebody allocates memory again.
*/

static slab_t slabs[SLAB_COUNT];

/* utility functions */

static int32_t find_free_object(int32_t slab_index);
static int32_t find_allocated_object(int32_t slab_index);

/* core functions */

// create array of slabs, which all are of different sizes
// ranging from 2 to 2048 but only power of 2

void *internal_mem_ptr = NULL;
size_t internal_mem_len = 0;
uint32_t page_alloc_count = 0;

void slab_init(void)
{
    for (int32_t i = 0; i < SLAB_COUNT; i++)
    {
        slabs[i].size = pow(2, i + 1);
        slabs[i].is_full = false;

        int32_t objects_per_slab = MAX_SLAB_SIZE / pow(2, i + 1);

        for (int j = 0; j < objects_per_slab; j++)
        {
            if (!internal_mem_ptr || internal_mem_ptr == (void *)internal_mem_len)
            {
                internal_mem_ptr = pmm_alloc(1);
                internal_mem_len = (size_t)internal_mem_ptr + PAGE_SIZE;

                page_alloc_count++;
            }

            slabs[i].objects[j] = internal_mem_ptr;
            internal_mem_ptr += slabs[i].size;
        }

        slabs[i].address_range.start = slabs[i].objects[0];
        slabs[i].address_range.end = slabs[i].objects[0] + 1024 - slabs[i].size;
    }

    serial_log(INFO, "Slab allocator statistics:\n");
    kernel_log(INFO, "Slab allocator statistics:\n");

    serial_set_color(TERM_PURPLE);

    debug("Slabs created: %d\n", SLAB_COUNT);
    printk(GFX_PURPLE, "Slabs created: %d\n", SLAB_COUNT);

    debug("Smallest slab: 2 bytes | Biggest slab: %d bytes\n", MAX_SLAB_SIZE);
    printk(GFX_PURPLE, "Smallest slab: 2 bytes | Biggest slab: %d bytes\n", MAX_SLAB_SIZE);

    debug("Memory used to create slabs: %d bytes = %d pages\n", page_alloc_count * PAGE_SIZE, page_alloc_count);
    printk(GFX_PURPLE, "Memory used to create slabs: %d bytes = %d pages\n", page_alloc_count * PAGE_SIZE, page_alloc_count);

    serial_set_color(TERM_COLOR_RESET);

    serial_log(INFO, "Slab allocator initialized\n");
    kernel_log(INFO, "Slab allocator initialized\n");
}

// search for a free object in the matching slab according to the
// size parameter and allocate it by setting it's value to NULL
void *slab_alloc(size_t size)
{
    void *return_value = NULL;

    for (int32_t i = 0; i < SLAB_COUNT; i++)
    {
        if (slabs[i].size != size)
            continue;

        if (slabs[i].is_full)
            return return_value;

        int32_t free_object_index = find_free_object(i);

        if (free_object_index == -1)
            slabs[i].is_full = true;
        else
        {
            return_value = slabs[i].objects[free_object_index];
            slabs[i].objects[free_object_index] = NULL;
        }

        return return_value;
    }

    return return_value;
}

// search for a slab of matching size according to the ptr paramater
// free it by setting it's value to the ptr which was previously checked
void slab_free(void *ptr)
{
    if (!ptr)
        return;

    void *ptr_without_base;

    for (int32_t i = 0; i < SLAB_COUNT; i++)
    {
        ptr_without_base = ptr - slabs[i].address_range.start + (void *)slabs[i].size;

        if (ptr >= slabs[i].address_range.start && ptr <= slabs[i].address_range.end)
        {
            // yes, ptr is in the range of a slab

            if ((size_t)ptr_without_base % slabs[i].size == 0)
            {
                // yes, ptr is an object of the slab

                int32_t allocated_object_index = find_allocated_object(i);

                if (allocated_object_index == -1)
                    return;

                slabs[i].objects[allocated_object_index] = ptr;
            }
        }
    }
}

/* utility functions */

// find an object in the matching slab, i.e. by searching for
// non-NULL values
static int32_t find_free_object(int32_t slab_index)
{
    int32_t objects_per_slab = MAX_SLAB_SIZE / pow(2, slab_index + 1);

    for (int32_t i = 0; i < objects_per_slab; i++)
        if (slabs[slab_index].objects[i] != NULL)
            return i;

    return -1;
}

// find an object in the matching slab, i.e. by searching for
// NULL values
static int32_t find_allocated_object(int32_t slab_index)
{
    int32_t objects_per_slab = MAX_SLAB_SIZE / pow(2, slab_index + 1);

    for (int32_t i = 0; i < objects_per_slab; i++)
        if (slabs[slab_index].objects[i] == NULL)
            return i;

    return -1;
}
