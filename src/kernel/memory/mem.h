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

#ifndef MEM_H
#define MEM_H

// NOTE: if I add 5 level paging change name to HIGHER_HALF_KERNEL_DATA_LV4 / 5
// UL signalises unsigned long to the compiler (can prevent ugly casts) 
#define HIGHER_HALF_KERNEL_DATA		0xFFFF800000000000UL
#define HIGHER_HALF_KERNEL_CODE		0xFFFFFFFF80000000UL

#define PAGE_SIZE				4096
#define TABLES_PER_DIRECTORY	512
#define PAGES_PER_TABLE			512		// do I need this???

#define KB_TO_PAGES(kb)			(((kb) * 1024) / PAGE_SIZE)
#define ALIGN_DOWN(addr, align)	((addr) & ~((align)-1))
#define ALIGN_UP(addr, align)	(((addr) + (align)-1) & ~((align)-1))

#define TO_VIRTUAL_ADDRESS(physical_address) (HIGHER_HALF_KERNEL_DATA + physical_address)		// convert an actual physical address to a higher half (kernel) data address
#define TO_PHYSICAL_ADDRESS(physical_address) (HIGHER_HALF_KERNEL_CODE + physical_address)		// convert an actual physical address to a higher half (kernel) code address
#define FROM_VIRTUAL_ADDRESS(virtual_address) (virtual_address - HIGHER_HALF_KERNEL_DATA)		// convert a higher half (kernel) data address to an actual physical address
#define FROM_PHYSICAL_ADDRESS(physical_address) (physical_address - HIGHER_HALF_KERNEL_CODE) 	// convert a higher half (kernel) code address to an actual physical address

#endif
