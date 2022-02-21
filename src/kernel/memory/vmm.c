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

#include <stdint.h>
#include <stddef.h>

#include <boot/stivale2.h>
#include <boot/stivale2_boot.h>
#include <memory/pmm.h>
#include <memory/vmm.h>
#include <libk/debug/debug.h>
#include <libk/log/log.h>
#include <libk/stdio/stdio.h>
#include <libk/string/string.h>

static PAGE_DIR root_page_directory;

// create and activate page directory + map important memory areas
void vmm_init(void)
{
    root_page_directory = vmm_create_page_directory();


    serial_log(INFO, "Paging - Multilevel support:\n");
    kernel_log(INFO, "Paging - Multilevel support:\n");


    serial_set_color(TERM_PURPLE);

    // check if la57 bit is enabled for 5-level paging
    if (is_la57_enabled())
    {
        debug("5-level paging supported!\n");
        printk(GFX_PURPLE, "5-level paging supported!\n");
    }
    else
    {
        debug("5-level paging not supported! Continuing with 4-level paging.\n");
        printk(GFX_PURPLE, "5-level paging not supported! Continuing with 4-level paging.\n");
    }

    serial_set_color(TERM_COLOR_RESET);


    serial_log(INFO, "Paging - Mapped areas:\n");
    kernel_log(INFO, "Paging - Mapped areas:\n");

    serial_set_color(TERM_PURPLE);

    // map first 4 GiB
    for (uint64_t i = 0; i < 4 * GB; i += PAGE_SIZE)
        vmm_map_page(root_page_directory, i, i, PTE_PRESENT | PTE_READ_WRITE);

    debug("1/3: Mapped first 4 GiB of memory\n");
    printk(GFX_PURPLE, "1/3: Mapped first 4 GiB of memory\n");

    // map higher half kernel address space
    for (uint64_t i = 0; i < 4 * GB; i += PAGE_SIZE)
        vmm_map_page(root_page_directory, i, phys_to_higher_half_data(i), PTE_PRESENT | PTE_READ_WRITE);

    debug("2/3: Mapped higher half kernel address space\n");
    printk(GFX_PURPLE, "2/3: Mapped higher half kernel address space\n");

    // map protected memory ranges (PMR's) - keep them read only for safety
    for (uint64_t i = 0; i < 0x80000000; i += PAGE_SIZE)
        vmm_map_page(root_page_directory, i, phys_to_higher_half_code(i), PTE_PRESENT);

    debug("3/3: Mapped protected memory ranges\n");
    printk(GFX_PURPLE, "3/3: Mapped protected memory ranges\n");

    serial_set_color(TERM_COLOR_RESET);


    vmm_activate_page_directory(root_page_directory);

    // no need to enable paging
    // (= set bit 31 in cr0)
    // as limine already handled that

    serial_log(INFO, "VMM initialized\n");
    kernel_log(INFO, "VMM initialized\n");
}

// set each table in the page directory to not used
PAGE_DIR vmm_create_page_directory(void)
{
    PAGE_DIR new_page_directory = pmm_alloc(1);

    // "clean" the page directory by setting everything to zero
    memset((void *)higher_half_data_to_phys((uint64_t)new_page_directory), 0, PAGE_SIZE);

    return new_page_directory;
}

// return a new page directory made with higher (X) page directory
static PAGE_DIR vmm_get_page_map_level(PAGE_DIR page_map_level_X, uintptr_t index_X, int flags)
{
    // NOTE: if you are unfamiliar with this syntax
    // *x = NULL;
    // x[i] = y
    // just means
    // *(x + i) = y

    if (page_map_level_X[index_X] & 1)
        return (PAGE_DIR)(page_map_level_X[index_X] & ~(511));
    else
    {
        page_map_level_X[index_X] = higher_half_data_to_phys((uint64_t)pmm_alloc(1)) | flags;

        return (PAGE_DIR)(page_map_level_X[index_X] & ~(511));
    }
}

// TODO: do more testing especially for 5-level paging

// map physical memory to virtual memory by using 4-level paging
void vmm_map_page(PAGE_DIR current_page_directory, uintptr_t physical_address, uintptr_t virtual_address, int flags)
{
    if (is_la57_enabled())	// 5-level paging is enabled
    {
        uintptr_t index5 = (virtual_address & ((uintptr_t)0x1ff << 48)) >> 48;
        uintptr_t index4 = (virtual_address & ((uintptr_t)0x1ff << 39)) >> 39;
        uintptr_t index3 = (virtual_address & ((uintptr_t)0x1ff << 30)) >> 30;
        uintptr_t index2 = (virtual_address & ((uintptr_t)0x1ff << 21)) >> 21;
        uintptr_t index1 = (virtual_address & ((uintptr_t)0x1ff << 12)) >> 12;

        PAGE_DIR page_map_level5 = current_page_directory;
        PAGE_DIR page_map_level4 = NULL;
        PAGE_DIR page_map_level3 = NULL;
        PAGE_DIR page_map_level2 = NULL;
        PAGE_DIR page_map_level1 = NULL;

        page_map_level4 = vmm_get_page_map_level(page_map_level5, index5, flags);
        page_map_level3 = vmm_get_page_map_level(page_map_level4, index4, flags);
        page_map_level2 = vmm_get_page_map_level(page_map_level3, index3, flags);
        page_map_level1 = vmm_get_page_map_level(page_map_level2, index2, flags);

        // if (page_map_level1[index1] != 0)
        // {
        // 	serial_log(ERROR, "Virtual page 0x%lX is already mapped to physical page 0x%lX! - page_map_level1[%d]: 0x%lX\n",
        // 			virtual_address, physical_address, index1, page_map_level1
        // 	);
        // 	serial_log(ERROR, "Kernel halted!\n");

        // 	kernel_log(ERROR, "Virtual page 0x%lX is already mapped to physical page 0x%lX! - page_map_level1[%d]: 0x%lX\n",
        // 			virtual_address, physical_address, index1, page_map_level1
        // 	);
        // 	kernel_log(ERROR, "Kernel halted!\n");

        // 	asm volatile("hlt");
        // }

        page_map_level1[index1] = physical_address | flags; // level 1 points to the mapped (physical) frame
    }
    else			// otherwise use standard 4-level paging
    {
        uintptr_t index4 = (virtual_address & ((uintptr_t)0x1ff << 39)) >> 39;
        uintptr_t index3 = (virtual_address & ((uintptr_t)0x1ff << 30)) >> 30;
        uintptr_t index2 = (virtual_address & ((uintptr_t)0x1ff << 21)) >> 21;
        uintptr_t index1 = (virtual_address & ((uintptr_t)0x1ff << 12)) >> 12;

        PAGE_DIR page_map_level4 = current_page_directory;
        PAGE_DIR page_map_level3 = NULL;
        PAGE_DIR page_map_level2 = NULL;
        PAGE_DIR page_map_level1 = NULL;

        page_map_level3 = vmm_get_page_map_level(page_map_level4, index4, flags);
        page_map_level2 = vmm_get_page_map_level(page_map_level3, index3, flags);
        page_map_level1 = vmm_get_page_map_level(page_map_level2, index2, flags);

        // if (page_map_level1[index1] != 0)
        // {
        // 	serial_log(ERROR, "Virtual page 0x%lX is already mapped to physical page 0x%lX! - page_map_level1[%d]: 0x%lX\n",
        // 			virtual_address, physical_address, index1, page_map_level1
        // 	);
        // 	serial_log(ERROR, "Kernel halted!\n");

        // 	kernel_log(ERROR, "Virtual page 0x%lX is already mapped to physical page 0x%lX! - page_map_level1[%d]: 0x%lX\n",
        // 			virtual_address, physical_address, index1, page_map_level1
        // 	);
        // 	kernel_log(ERROR, "Kernel halted!\n");

        // 	asm volatile("hlt");
        // }

        page_map_level1[index1] = physical_address | flags; // level 1 points to the mapped (physical) frame
    }

    vmm_flush_tlb((void *)virtual_address);
}

void vmm_unmap_page(PAGE_DIR current_page_directory, uintptr_t virtual_address)
{
    if (is_la57_enabled())	// 5-level paging is enabled
    {
        uintptr_t index5 = (virtual_address & ((uintptr_t)0x1ff << 48)) >> 48;
        uintptr_t index4 = (virtual_address & ((uintptr_t)0x1ff << 39)) >> 39;
        uintptr_t index3 = (virtual_address & ((uintptr_t)0x1ff << 30)) >> 30;
        uintptr_t index2 = (virtual_address & ((uintptr_t)0x1ff << 21)) >> 21;
        uintptr_t index1 = (virtual_address & ((uintptr_t)0x1ff << 12)) >> 12;

        PAGE_DIR page_map_level5 = current_page_directory;
        PAGE_DIR page_map_level4 = NULL;
        PAGE_DIR page_map_level3 = NULL;
        PAGE_DIR page_map_level2 = NULL;
        PAGE_DIR page_map_level1 = NULL;

        // flag argument is set to zero because it won't be used
        page_map_level4 = vmm_get_page_map_level(page_map_level5, index5, 0);
        page_map_level3 = vmm_get_page_map_level(page_map_level4, index4, 0);
        page_map_level2 = vmm_get_page_map_level(page_map_level3, index3, 0);
        page_map_level1 = vmm_get_page_map_level(page_map_level2, index2, 0);

        page_map_level1[index1] = 0;
    }
    else			// otherwise use standard 4-level paging
    {
        uintptr_t index4 = (virtual_address & ((uintptr_t)0x1ff << 39)) >> 39;
        uintptr_t index3 = (virtual_address & ((uintptr_t)0x1ff << 30)) >> 30;
        uintptr_t index2 = (virtual_address & ((uintptr_t)0x1ff << 21)) >> 21;
        uintptr_t index1 = (virtual_address & ((uintptr_t)0x1ff << 12)) >> 12;

        PAGE_DIR page_map_level4 = current_page_directory;
        PAGE_DIR page_map_level3 = NULL;
        PAGE_DIR page_map_level2 = NULL;
        PAGE_DIR page_map_level1 = NULL;

        // flag argument is set to zero because it won't be used
        page_map_level3 = vmm_get_page_map_level(page_map_level4, index4, 0);
        page_map_level2 = vmm_get_page_map_level(page_map_level3, index3, 0);
        page_map_level1 = vmm_get_page_map_level(page_map_level2, index2, 0);

        page_map_level1[index1] = 0;
    }

    vmm_flush_tlb((void *)virtual_address);
}

// invalidate a single page in the translation lookaside buffer
void vmm_flush_tlb(void *address)
{
    asm volatile("invlpg (%0)" : : "r" (address));
}

// write the page directory address to cr3
void vmm_activate_page_directory(PAGE_DIR current_page_directory)
{
    asm volatile("mov %0, %%cr3" : : "r" (higher_half_data_to_phys((uint64_t)current_page_directory)) : "memory");
}
