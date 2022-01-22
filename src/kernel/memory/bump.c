/*
	This file is part of an x86_64 hobbyist operating system called KnutOS
	Everything is openly developed on GitHub: https://github.com/Tix3Dev/KnutOS/

	Copyright (C) 2021  Yves Vollmeier <https://github.com/Tix3Dev>
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

static int mmap_idx = 0;
static int mmap_idx_max = 0;

void *mmap_alloc(struct stivale2_struct *stivale2_struct, size_t size)
{
    struct stivale2_struct_tag_memmap *stivale2mmap = stivale2_get_tag(stivale2_struct,
            STIVALE2_STRUCT_TAG_MEMMAP_ID);

	size_t entry_sz = stivale2mmap->memmap[mmap_idx].length;

	if (entry_sz < size)
	{
		if (mmap_idx == mmap_idx_max)
			return NULL;

		mmap_idx++;
		mmap_alloc(stivale2_struct, size);
	}

	size_t ret = stivale2mmap->memmap[mmap_idx].base;
	stivale2mmap->memmap[mmap_idx].base += size;
	stivale2mmap->memmap[mmap_idx].length -= size;
	return (void *)ret;
}
