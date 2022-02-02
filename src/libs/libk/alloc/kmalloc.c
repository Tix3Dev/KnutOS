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

// #include <stddef.h>
// #include <stdint.h>
//
// #include <memory/mem.h>
// #include <memory/pmm.h>
// #include <memory/slab.h>
//
// void *kmalloc(size_t size)
// {
//     if (size >= PAGE_SIZE)
//     {
// 	// pmm_alloc(page_count)
//     }
//     else
//     {
// 	// slab_alloc(bytes)
//     }
// }
//
// void kfree(void *ptr)
// {
//     // TODO: would have to change pmm_free because it needs a size as parameter
// }
