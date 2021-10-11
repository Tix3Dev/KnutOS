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

#include <stdint.h>
#include <stddef.h>

#include <boot/stivale2.h>
#include <memory/pmm.h>
#include <memory/vmm.h>

VMM_INFO_t *root;

void vmm_init(void)
{
	root = vmm_create_page_directory();

	for (int i = 0; i < PAGE_SIZE * PAGES_PER_TABLE; i += PAGE_SIZE)
		vmm_map_page(root, i, i, PTE_PRESENT | PTE_READ_WRITE);

	vmm_activate_page_directory(root);

	// no need to enable paging
	// (= set bit 31 in cr0)
	// as limine already handled that
}

// set each table in the page directory to not used
VMM_INFO_t *vmm_create_page_directory(void)
{
	VMM_INFO_t *new_vmm = pmm_alloc(1);

	new_vmm->page_directory = pmm_alloc(1);

	for (int i = 0; i < TABLES_PER_DIRECTORY; i++)
		new_vmm->page_directory[i] = 0;

	return new_vmm;
}

void vmm_map_page(VMM_INFO_t *vmm, uintptr_t physical_address, uintptr_t virtual_address, int flags)
{
	uintptr_t index4 = (virtual_address & ((uintptr_t)0x1ff << 39)) >> 39;
	uintptr_t index3 = (virtual_address & ((uintptr_t)0x1ff << 30)) >> 30;
	uintptr_t index2 = (virtual_address & ((uintptr_t)0x1ff << 21)) >> 21;
	uintptr_t index1 = (virtual_address & ((uintptr_t)0x1ff << 12)) >> 12;

	uint64_t *page_map_level4 = vmm->page_directory;
	uint64_t *page_map_level3 = NULL;
	uint64_t *page_map_level2 = NULL;
	uint64_t *page_map_level1 = NULL;

	if (*page_map_level4 & 1)
		// old: page_map_level3 = (uint64_t *)page_map_level4[index4];
		page_map_level3 = (uint64_t *)(page_map_level4[index4] & ~(511));
	else
		// old: page_map_level3[index3] = (uint64_t)pmm_alloc(1);
		page_map_level3[index3] = (uint64_t)pmm_alloc(1) | flags;

	if (*page_map_level3 & 1)
		page_map_level2 = (uint64_t *)page_map_level3[index3];
	else
		page_map_level2[index2] = (uint64_t)pmm_alloc(1);

	if (*page_map_level2 & 1)
		page_map_level1 = (uint64_t *)page_map_level2[index2];
	else
		page_map_level1[index1] = (uint64_t)pmm_alloc(1);

	page_map_level1[index1] = physical_address | flags; // level 1 points to the mapped (physical) frame

	vmm_flush_tlb((void *)virtual_address);
}

// invalidate a single page in the translation lookaside buffer
void vmm_flush_tlb(void *address)
{
	asm volatile("invlpg (%0)" : : "r" (address));
}

// write the page directory address to cr3
void vmm_activate_page_directory(VMM_INFO_t *vmm)
{
	asm volatile("mov %0, %%cr3" : : "r" (vmm->page_directory) : "memory");
}
