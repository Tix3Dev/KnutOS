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
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <boot/stivale2.h>
#include <boot/stivale2_boot.h>
#include <memory/mem.h>
#include <memory/pmm.h>
#include <memory/slab.h>
#include <libk/alloc/kmalloc.h>

#include <libk/debug/debug.h>

size_t next_pow_of_two(size_t num);

// use pmm_alloc (page based) for big size parameters
// use slab_alloc (byte based) for small size parameters
void *kmalloc(size_t size)
{
    void *ptr = NULL;

    size_t new_size = next_pow_of_two(size);

    if (new_size >= PAGE_SIZE)
    {
        debug("kmalloc(%d) rounded to %d (+ metadata %d) - big alloc\n", size, new_size, PAGE_SIZE);

        ptr = pmm_alloc((new_size / PAGE_SIZE) + 1);

        kmalloc_metadata_t *metadata = ptr;
        metadata->size = new_size;

        ptr += PAGE_SIZE;

        debug("(big alloc) alloc size: %d\n", metadata->size);
    }
    else
    {
        debug("kmalloc(%d) rounded to %d (+ metadata %d) - small alloc\n", size, new_size, sizeof(kmalloc_metadata_t));

        new_size = next_pow_of_two(size + sizeof(kmalloc_metadata_t));

        ptr = slab_alloc(new_size);

        kmalloc_metadata_t *metadata = ptr;
        metadata->size = new_size;

        ptr += sizeof(kmalloc_metadata_t);

        debug("(small alloc) alloc size: %d\n", metadata->size);
    }

    debug("allocated memory at: 0x%.16llx\n", ptr);

    return ptr;

}

// find out if the pointers belongs to a pmm_alloc or a slab_alloc
// use pmm_free / slab_alloc accordingly
void kfree(void *ptr)
{
    if (!ptr)
        return;

    // using AND 0xFFF will return whether the last three digits (in hex) are zero
    // if yes big alloc has been used
    // OUTDATED: if no small alloc has been used and we need to toggle them to zero by
    // OUTDATED: using AND ~0xFF
    // OUTDATED: see bottom of file to see how it works using an example

    if (((uint64_t)ptr & 0xFFF) == 0)
    {
        kmalloc_metadata_t *metadata = ptr - PAGE_SIZE;

        pmm_free(metadata, (metadata->size / 4096) + 1);


        debug("free size: %d\n", metadata->size);
        debug("(big free) freed memory at: 0x%.16llx\n", ptr);

        return;
    }

    // OUTDATED: ptr = (void *)((uint64_t)ptr & ~(0xFFF));

    kmalloc_metadata_t *metadata = ptr - sizeof(kmalloc_metadata_t);

    slab_free(metadata); // we don't need metadata->size, automatically found by slab_free

    debug("free size: %d\n", metadata->size);
    debug("(small free) freed memory at: 0x%.16llx\n", ptr);
}

// get the next power of two
size_t next_pow_of_two(size_t num)
{
    // TODO: might want to make this more efficient
    // x <<= 1 is at least better than x *= 2
    // note the result for 0 and 1 is 2
    // note after this size will be page aligned
    size_t result = 2;

    while (result < num)
        result <<= 1;

    return result;
}

/* Example for bitwise manipulation used to determine whether big alloc or small alloc has been used */

/*  main.c
    #include <stdio.h>
    #include <stdint.h>

    int main() {
    uint64_t ptr = 0xffff80000169f000;

    printf("first:\t%lx\n", ptr);

    if (((uint64_t)ptr & 0xfff) == 0)
      printf("true\n");

    printf("second:\t%lx\n", ptr);

    ptr = (uint64_t)ptr & ~(0xfff);

    printf("third:\t%lx\n", ptr);

    return 0;
    }
*/

/*  output
    first:  ffff80000169f000
    true
    second: ffff80000169f000
    third:  ffff80000169f000
*/
