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

void *kmalloc(size_t size)
{
    // debug("kmalloc(%d) rounded to %d - ", size, next_pow_of_two);

    void *ptr = NULL;
    size_t new_size = next_pow_of_two(size);

    if (new_size >= PAGE_SIZE)
    {
	debug("kmalloc(%d) rounded to %d - big alloc", size, new_size);
	ptr = pmm_alloc((new_size / PAGE_SIZE) + 1);
    }
    else
    {
	new_size = next_pow_of_two(size + sizeof(kmalloc_metadata_t));
	debug("kmalloc(%d) rounded to %d - small alloc", size, new_size);
	ptr = slab_alloc(new_size);
    }

    if (!ptr)
	return NULL;

    ptr = phys_to_higher_half_data((uintptr_t)ptr);

    kmalloc_metadata_t *metadata = ptr;
    metadata->size = new_size;

    if (new_size >= PAGE_SIZE)
	ptr += PAGE_SIZE;
    else
	ptr += sizeof(kmalloc_metadata_t);

    return ptr;
}

void kfree(void *ptr)
{
    if (!ptr)
	return;
}

size_t next_pow_of_two(size_t num)
{
    // TODO: might want to make this more efficient
    // x <<= 1 is at least better than x *= 2
    // note the result for 0 and 1 is
    // note after this size will be page aligned
    size_t result = 2;
    while (result < num)
	result <<= 1;

    return result;
}
