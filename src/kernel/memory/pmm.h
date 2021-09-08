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

#ifndef PMM_H
#define PMM_H

#define BLOCK_SIZE			4096
#define KB_TO_BLOCKS(kb)	(((kb) * 1024) / BLOCK_SIZE)
#define BIT_FREE			0
#define BIT_USED			1

struct PMM_Info_Struct
{
	size_t		memory_size;
	uint32_t	max_blocks;
	uint32_t	used_blocks;
	struct		stivale2_struct_tag_memmap *memory_map;
};

void pmm_init(struct stivale2_struct *stivale2_struct);
const char *get_memory_map_entry_type(uint32_t type);
int pmm_find_first_free_block(void);
void *pmm_alloc(void);
void pmm_free(void *pointer);

#endif
