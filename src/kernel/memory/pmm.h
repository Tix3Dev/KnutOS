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

#include <memory/mem.h>

#ifndef PMM_H
#define PMM_H

struct PMM_Info_Struct
{
    size_t	memory_size;
    uint32_t	max_pages;
    uint32_t	used_pages;
    struct	stivale2_struct_tag_memmap *memory_map;
};

void pmm_init(struct stivale2_struct *stivale2_struct);
const char *get_memory_map_entry_type(uint32_t type);
void *pmm_find_first_free_page(size_t page_count);
void *pmm_alloc(size_t page_count);
void pmm_free(void *pointer, size_t page_count);

#endif
