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
#include <libk/string/string.h>

struct	PMM_Info_Struct	pmm_info;
BITMAP_t				bitmap;

size_t	highest_page;

// setup the bitmap
void pmm_init(struct stivale2_struct *stivale2_struct)
{
	// --- step 1 ---

	// set basic values
	struct stivale2_struct_tag_memmap *memory_map = stivale2_get_tag(stivale2_struct,
			STIVALE2_STRUCT_TAG_MEMMAP_ID);

	pmm_info.memory_map		= memory_map;

	struct stivale2_mmap_entry *current_entry;


	// --- step 2 ---

	log(INFO, "Memory map layout:\n");

	size_t top = 0;

	// iterate through memory map entries and print info about them
	// & find highest page
	for (uint64_t i = 0; i < pmm_info.memory_map->entries; i++)
	{
		current_entry = &pmm_info.memory_map->memmap[i];


		serial_set_color(TERM_PURPLE);
		debug("Memory map entry No. %.16d: Base: 0x%.16llx | Length: 0x%.16llx | Type: %s\n",
			  i, current_entry->base, current_entry->length, get_memory_map_entry_type(current_entry->type));
		serial_set_color(TERM_COLOR_RESET);


		if (current_entry->type != STIVALE2_MMAP_USABLE &&
				current_entry->type != STIVALE2_MMAP_BOOTLOADER_RECLAIMABLE &&
				current_entry->type != STIVALE2_MMAP_KERNEL_AND_MODULES)
			continue;

		top = current_entry->base + current_entry->length;

		if (top > highest_page)
			highest_page = top;
	}

	pmm_info.memory_size	= highest_page;
	pmm_info.max_pages		= KB_TO_PAGES(pmm_info.memory_size);
	pmm_info.used_pages	= pmm_info.max_pages;


	// --- step 3 ---

	// calculate bitmap size
	size_t bitmap_byte_size = ALIGN_UP(ALIGN_DOWN(highest_page, PAGE_SIZE) / PAGE_SIZE / 8, PAGE_SIZE);

	bitmap.size = bitmap_byte_size;

	log(INFO, "Memory specifications:\n");

	serial_set_color(TERM_PURPLE);
	current_entry = &pmm_info.memory_map->memmap[0];
	debug("Total amount of memory: %d kB\n", current_entry->base + current_entry->length - 1);
	debug("Size of bitmap: %d kB\n", bitmap.size / 1024);
	serial_set_color(TERM_COLOR_RESET);


	// --- step 4 ---

	// search for first large enough page of memory to host the bitmap
	for (uint64_t i = 0; i < pmm_info.memory_map->entries; i++)
	{
		current_entry = &pmm_info.memory_map->memmap[i];

		if (current_entry->type == STIVALE2_MMAP_USABLE)
			continue;

		if (current_entry->length >= bitmap.size)
		{
			serial_set_color(TERM_PURPLE);
			debug("Found big enough block of memory to host the bitmap!\n");
			debug("Bitmap stored between 0x%.8lx and 0x%.8lx\n", current_entry->base, current_entry->base + current_entry->length - 1);
			serial_set_color(TERM_COLOR_RESET);

			bitmap.map				= (uint8_t *)(TO_VIRTUAL_ADDRESS(current_entry->base));

			current_entry->base		+= bitmap.size;
			current_entry->length	-= bitmap.size;

			break;
		}
	}


	// --- step 5 ---

	// set bitmap to default - all bits used
	memset((void *)bitmap.map, 0xFF, bitmap.size);


	// --- step 6 ---

	// set all usable entries in the bitmap to free
	for (uint64_t i = 0; i < pmm_info.memory_map->entries; i++)
	{
		current_entry = &pmm_info.memory_map->memmap[i];

		if (current_entry->type == STIVALE2_MMAP_USABLE)
			pmm_free((void *)current_entry->base, current_entry->length / PAGE_SIZE);
	}


	// --- step 7 ---

	// reserve the null pointer
	bitmap_set_bit(&bitmap, 0);


	// --- done ---

	log(INFO, "PMM initialized\n");
}

// return matching string for memory map entry type passed
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
// return index
void *pmm_find_first_free_page(size_t page_count)
{
	// can't find in no memory
	if (page_count == 0)
		return NULL;

	for (size_t counter = 0; counter < page_count; counter++)
	{
		for (size_t i = 0; i < PAGE_TO_BIT(highest_page); i++)
		{
			if (!bitmap_check_bit(&bitmap, i))
				return (void *)BIT_TO_PAGE(i);
		}
	}

	// nothing big enough
	return NULL;
}

// check if there is still memory left
// find first free page
// mark it as used
// return address
// -> physical memory allocation for n pages
void *pmm_alloc(size_t page_count)
{
	if (pmm_info.used_pages <= 0)
		return NULL;

	void *pointer = pmm_find_first_free_page(page_count);

	if (pointer == NULL)
		return NULL;


	uint64_t index = (uint64_t)pointer / PAGE_SIZE;

	for (size_t i = 0; i < page_count; i++)
		bitmap_set_bit(&bitmap, index + i);

	pmm_info.used_pages += page_count;

	return (void *)(uint64_t)(TO_VIRTUAL_ADDRESS(index * PAGE_SIZE));
}

// convert pointer to index
// unset the matching bit
// -> physical memory freeing for n pages
void pmm_free(void *pointer, size_t page_count)
{
	uint64_t index = FROM_VIRTUAL_ADDRESS((uint64_t)pointer) / PAGE_SIZE;

	for (size_t i = 0; i < page_count; i++)
		bitmap_unset_bit(&bitmap, index + i);

	pmm_info.used_pages -= page_count;
}
