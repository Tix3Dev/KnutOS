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

#include <memory/pmm.h>

VMM_INFO_t *kernel_vmm;

void vmm_init(void)
{
	vmm_create_page_directory();

	for (int i = 0; i < PAGE_SIZE * PAGES_PER_TABLE; i += PAGE_SIZE)
		vmm_map_page();

	vmm_activate_page_directory();

	vmm_enable_paging();
}

// set each table in the page directory to not used
VMM_INFO_t *vmm_create_page_directory(void)
{
	VMM_INFO_t *new_vmm = pmm_alloc(1);

	new_vmm.page_directory = pmm_alloc(1);
	for (int i = 0; i < TABLES_PER_DIRECTORY; i++)
		new_vmm.page_directory[i] = 0;

	return new_vmm;
}

void vmm_map_page(VMM_INFO_t *vmm, uintptr_t physical_address, uintptr_t virtual_address)
{
	//
}

// write the page directory address to cr3
void vmm_activate_page_directory(VMM_INFO_t *vmm)
{
	asm volatile("mov %0, %%cr3" : : "r" (vmm->page_directory));
}

// set paging-bit (bit 31) in cr0
void vmm_enable_paging(void)
{
	uint32_t cr0;

	asm volatile("mov %%cr0, %0" : "=r" (cr0));
	cr0 |= (1 << 31);
	asm volatile("mov %0, %%cr0" : : "r" (cr0));
}
