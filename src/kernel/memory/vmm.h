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

#include <memory/mem.h>

#ifndef VMM_H
#define VMM_H

#define GB 0x40000000UL

// page table entry flags
#define PTE_PRESENT				1
#define PTE_READ_WRITE			2
#define PTE_USER_SUPERVISOR		4
#define PTE_WRITE_THROUGH		8
#define PTE_CHACHE_DISABLED		16
#define PTE_ACCESSED			32
#define PTE_DIRTY				64
#define PTE_PAT					128
#define PTE_GLOBAL				256

typedef uint64_t * PAGE_DIR;

void vmm_init(struct stivale2_struct *stivale2_struct);
PAGE_DIR vmm_create_page_directory(void);
void vmm_map_page(PAGE_DIR vmm, uintptr_t physical_address, uintptr_t virtual_address, int flags);
uint64_t *vmm_get_page_map_level(PAGE_DIR page_map_level_X, uintptr_t index_X, int flags);
void vmm_flush_tlb(void *address);
void vmm_activate_page_directory(PAGE_DIR vmm);

#endif
