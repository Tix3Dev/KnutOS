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

#ifndef KMALLOC_H
#define KMALLOC_H

typedef struct
{
    size_t size;
} kmalloc_metadata_t;

void *kmalloc(size_t size);
void kfree(void *ptr);

void *kmalloc(size_t size)
{
    void *ptr = NULL;

    size_t new_size = next_pow_of_two(size);

    if (new_size >= PAGE_SIZE)
    {
	ptr = pmm_alloc((new_size / PAGE_SIZE) + 1);
	ptr = (size_t)phys_to_higher_half_data((uintptr_t)ptr);

	kmalloc_metadata_t *metadata = ptr;
	metadata->size = new_size;

	ptr += PAGE_SIZE;
    }
    else
    {
	// slab
    }

    return ptr;

}

void kfree(void *ptr)
{
    if (!ptr)
	return;

    if ((ptr & 0xFFFF) == 0) // check this
    { // check all this
	kmalloc_metadata_t *metadata = ptr - PAGE_SIZE;
	metadata = (kmalloc_metadata_t)higher_half_data_to_phys((uintptr_t)metadata);

	pmm_free(metadata, (metadata->size / 4096) + 1);
	
	return;
    }
    else
    {
	// slab
    }
}

#endif
