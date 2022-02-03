/*
	This file is part of an x86_64 hobbyist operating system called KnutOS
	Everything is openly developed on GitHub: https://github.com/Tix3Dev/KnutOS/

	Copyright (C) 2021-2022  Yves Vollmeier <https://github.com/Tix3Dev>
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.

	Contributor(s): Tim Thompson <https://github.com/V01D-NULL>
*/

/*
	The first version was developed by Yves Vollmeier and Tim Thompson and the
	code can be seen here:
	https://github.com/V01D-NULL/slab_allocator
	The second version was mainly developed by Tim Thompson and modified by
	Yves Vollmeier and the code can be seen here:
	https://github.com/V01D-NULL/3ds-firmware/blob/main/src/arm11/core/mm/slab.c
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <boot/stivale2.h>
#include <boot/stivale2_boot.h>
#include <memory/bump.h>
#include <memory/slab.h>
#include <libk/math/math.h>

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
// ranging from 2 to 512 but only power of 2
void slab_init(struct stivale2_struct *stivale2_struct)
{
    for (int32_t i = 0; i < SLAB_COUNT; i++)
    {
        slabs[i].size = pow(2, i + 1);
        slabs[i].is_full = false;

        int32_t objects_per_slab = MAX_SLAB_SIZE / pow(2, i + 1);

        for (int j = 0; j < objects_per_slab; j++)
            slabs[i].objects[j] = bump_alloc(stivale2_struct, slabs[i].size);

        slabs[i].address_range.start = slabs[i].objects[0];
        slabs[i].address_range.end = slabs[i].objects[0] + 1024 - slabs[i].size;
    }
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
