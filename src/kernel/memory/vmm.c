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
#include <boot/stivale2_boot.h>
#include <memory/pmm.h>
#include <memory/vmm.h>
#include <libk/debug/debug.h>
#include <libk/log/log.h>

VMM_INFO_t *root;

void vmm_init(struct stivale2_struct *stivale2_struct)
{
	struct stivale2_struct_tag_memmap *memory_map = stivale2_get_tag(stivale2_struct,
			STIVALE2_STRUCT_TAG_MEMMAP_ID);

	root = vmm_create_page_directory();


	log(INFO, "Paging - Mapped areas:\n");

	serial_set_color(TERM_PURPLE);

	// map first 4 GiB
	for (uint64_t i = 0; i < 4 * GB; i += PAGE_SIZE)
		vmm_map_page(root, i, i, PTE_PRESENT | PTE_READ_WRITE);

	debug("1/4: Mapped first 4 GiB of memory");

	// map higher half kernel address space
	for (uint64_t i = 0; i < 4 * GB; i += PAGE_SIZE)
		vmm_map_page(root, i, TO_VIRTUAL_ADDRESS(i), PTE_PRESENT | PTE_READ_WRITE);

	debug("2/4: Mapped higher half kernel address space\n");

	// map protected memory ranges (PMR's)
	for (uint64_t i = 0; i < 0x80000000; i += PAGE_SIZE)
		vmm_map_page(root, i, TO_PHYSICAL_ADDRESS(i), PTE_PRESENT | PTE_READ_WRITE);

	debug("3/4: Mapped protected memory ranges\n");

	// stivale2 structs
	for (uint64_t i = 0; i < memory_map->entries; i++)
	{
		struct stivale2_mmap_entry *current_entry = &memory_map->memmap[i];

		if (current_entry->type == STIVALE2_MMAP_USABLE)
		{
			for (uint64_t j = 0; j < memory_map->memmap[i].length; j += PAGE_SIZE)
				vmm_map_page(root, TO_VIRTUAL_ADDRESS(j), j, PTE_PRESENT | PTE_READ_WRITE);
		}
	}

	debug("4/4: Mapped stivale2 structs\n");

	serial_set_color(TERM_COLOR_RESET);


	vmm_activate_page_directory(root);

	// no need to enable paging
	// (= set bit 31 in cr0)
	// as limine already handled that

	log(INFO, "VMM initialized\n");
}

// set each table in the page directory to not used
VMM_INFO_t *vmm_create_page_directory(void)
{
	VMM_INFO_t *new_vmm = pmm_alloc(1);

	new_vmm->page_directory = pmm_alloc(1);

	// not needed since pmm_alloc returns already "clean" page (memset with 0)
	// for (int i = 0; i < TABLES_PER_DIRECTORY; i++)
	// 	new_vmm->page_directory[i] = 0;

	return new_vmm;
}

// map physical memory to virtual memory by using 4 level paging
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

	// NOTE: if you are unfamiliar with this syntax
	// *x = NULL;
	// x[i] = y
	// just means
	// *(x + i) = y

	if (page_map_level4[index4] & 1)
		page_map_level3 = (uint64_t *)(page_map_level4[index4] & ~(511));
	else
	{
		// page_map_level3[index3] = FROM_VIRTUAL_ADDRESS((uint64_t)pmm_alloc(1)) | flags;
		page_map_level4[index4] = FROM_VIRTUAL_ADDRESS((uint64_t)pmm_alloc(1)) | flags;

		page_map_level3 = (uint64_t *)(page_map_level4[index4] & ~(511));
	}

	if (page_map_level3[index3] & 1)
		page_map_level2 = (uint64_t *)(page_map_level3[index3] & ~(511));
	else
	{
		// page_map_level2[index2] = FROM_VIRTUAL_ADDRESS((uint64_t)pmm_alloc(1)) | flags;
		page_map_level3[index3] = FROM_VIRTUAL_ADDRESS((uint64_t)pmm_alloc(1)) | flags;

		page_map_level2 = (uint64_t *)(page_map_level3[index3] & ~(511));
	}

	if (page_map_level2[index2] & 1)
		page_map_level1 = (uint64_t *)(page_map_level2[index2] & ~(511));
	else
	{
		// page_map_level1[index1] = FROM_VIRTUAL_ADDRESS((uint64_t)pmm_alloc(1)) | flags;
		page_map_level2[index2] = FROM_VIRTUAL_ADDRESS((uint64_t)pmm_alloc(1)) | flags;

		page_map_level1 = (uint64_t *)(page_map_level2[index2] & ~(511));
	}

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
	asm volatile("mov %0, %%cr3" : : "r" (FROM_VIRTUAL_ADDRESS((uint64_t)vmm->page_directory)) : "memory");
}
