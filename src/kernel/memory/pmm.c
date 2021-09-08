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
#include <libk/alloc/bitmap.h>
#include <libk/debug/debug.h>
#include <libk/log/log.h>

struct PMM_Info_Struct pmm_info;

void pmm_init(struct stivale2_struct *stivale2_struct)
{
	struct stivale2_struct_vmap *vmap_info = stivale2_get_tag(stivale2_struct,
			STIVALE2_STRUCT_TAG_VMAP);

	pmm_info.memory_size	= 1024 + vmap_info->addr;
	pmm_info.max_blocks		= KB_TO_BLOCKS(pmm_info.memory_size);
	pmm_info.used_blocks	= pmm_info.max_blocks;
	pmm_info.memory_map		= stivale2_get_tag(stivale2_struct,
							  STIVALE2_STRUCT_TAG_MEMMAP_ID);

	struct stivale2_mmap_entry *current_entry;

	for (uint64_t i = 0; i < pmm_info.memory_map->entries; i++)
	{
		current_entry = &pmm_info.memory_map->memmap[i];

		serial_set_color(TERM_PURPLE);
		debug("Memory map entry No. %.16d: Base: 0x%.16llx | Length: 0x%.16llx | Type: %s\n",
			  i, current_entry->base, current_entry->length, get_memory_map_entry_type(current_entry->type));

		if (current_entry->type != STIVALE2_MMAP_USABLE &&
				current_entry->type != STIVALE2_MMAP_BOOTLOADER_RECLAIMABLE &&
				current_entry->type != STIVALE2_MMAP_KERNEL_AND_MODULES)
			continue;
	}

	log(INFO, __FILE__, "PMM initialized\n");
}

const char *get_memory_map_entry_type(uint32_t type)
{
	switch (type)
	{
		case STIVALE2_MMAP_USABLE:
			return "Usable";

		case STIVALE2_MMAP_RESERVED:
			return "Reserved";

		case STIVALE2_MMAP_ACPI_RECLAIMABLE:
			return "ACPI Reclaimable";

		case STIVALE2_MMAP_ACPI_NVS:
			return "ACPI Non Volatile Storage";

		case STIVALE2_MMAP_BAD_MEMORY:
			return "Bad Memory";

		case STIVALE2_MMAP_BOOTLOADER_RECLAIMABLE:
			return "Bootloader Reclaimable";

		case STIVALE2_MMAP_KERNEL_AND_MODULES:
			return "Kernel And Modules";

		case STIVALE2_MMAP_FRAMEBUFFER:
			return "Framebuffer";

		default:
			return "Unknown";
	}
}

// traverse the bitmap -> for each bit and check if bit is free or used
int pmm_find_first_free_block(void)
{
	for (uint32_t i = 0; i < pmm_info.used_blocks / 32; i++)
	{
		if (pmm_info.memory_map->memmap[i].base != 0xFFFFFFFF)
		{
			for (int j = 0; j < 32; j++)
			{
				int32_t bit = 1 << j;

				if (!(pmm_info.memory_map->memmap[i].base & bit))
					return i * 32 + j;
			}
		}
	}

	return -1;
}

// check if there is still memory left
// find first free block
// mark it as used
// return address
// -> physical memory allocation
void *pmm_alloc(void)
{
	if (pmm_info.used_blocks <= 0)
		return 0;

	int index = pmm_find_first_free_block();

	if (index == -1)
		return 0;

	bitmap_set_bit(pmm_info.memory_map, index);
	pmm_info.used_blocks++;

	return (void *)(uint64_t)(index * BLOCK_SIZE);
}

// convert pointer to index
// unset the matching bit
// -> physical memory freeing
void pmm_free(void *pointer)
{
	uint64_t index = (uint64_t)pointer / BLOCK_SIZE;

	bitmap_unset_bit(pmm_info.memory_map, index);
	pmm_info.used_blocks--;
}
