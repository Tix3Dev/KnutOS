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
#define MEM_h

#define MEMORY_OFFSET			0xFFFF800000000000

#define PAGE_SIZE				4096
#define TABLES_PER_DIRECTORY	1024
#define PAGES_PER_TABLE			1024									// do i need this???

#define KB_TO_PAGES(kb)			(((kb) * 1024) / PAGE_SIZE)
#define ALIGN_DOWN(addr, align)	((addr) & ~((align)-1))
#define ALIGN_UP(addr, align)	(((addr) + (align)-1) & ~((align)-1))

#endif
