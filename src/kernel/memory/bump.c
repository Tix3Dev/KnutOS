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

#include <stddef.h>
#include <stdbool.h>

#include <boot/stivale2.h>
#include <boot/stivale2_boot.h>
#include <memory/bump.h>

void *stivale2_mmap_alloc(struct stivale2_mmap_entry *entry, size_t size)
{
    entry->base += size;
    entry->length -= size;

    return (void *)entry->base;
}

void *bump_alloc(struct stivale2_struct *stivale2_struct, size_t size)
{
    struct stivale2_struct_tag_memmap *memory_map = stivale2_get_tag(stivale2_struct,
            STIVALE2_STRUCT_TAG_MEMMAP_ID);

    struct stivale2_mmap_entry *current_entry;

    for (uint64_t i = 0; i < memory_map->entries; i++)
    {
        current_entry = &memory_map->memmap[i];

        if (current_entry->type != STIVALE2_MMAP_USABLE)
            continue;

        if (current_entry->length >= size)
            return stivale2_mmap_alloc(current_entry, size);
    }

    return NULL;
}
