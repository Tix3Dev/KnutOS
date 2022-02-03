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

	Contributor(s): Tim Thompson <https://github.com/V01D-NULL>
*/

/*
	The first version was developed by Yves Vollmeier and Tim Thompson and the
	code can be seen here:
	https://github.com/V01D-NULL/slab_allocator
	The second version was mainly developed by Tim Thompson and modified by
	Yves Vollmeier and the code can be seen here:
	https://github.com/V01D-NULL/3ds-firmware/blob/main/src/arm11/core/mm/slab.c
*/

#include <stdbool.h>

#include <libk/definitions.h>

#ifndef SLAB_H
#define SLAB_H

#define SLAB_COUNT		10	// how many slab_t's there will be
#define MAX_SLAB_SIZE		1024	// until pow(2, SLAB_COUNT)
#define MAX_OBJECTS_PER_SLAB	512	// smallest slab possible is 2 -> MAX_SLAB_SIZE / 2

// an object in the objects array either is a pointer to
// free memory, or NULL which means it's allocated
typedef struct
{
    addr_range_t    address_range;

    size_t  size;
    bool    is_full;
    void    *objects[MAX_OBJECTS_PER_SLAB];
} slab_t;

void slab_init(struct stivale2_struct *stivale2_struct);
void *slab_alloc(size_t size);
void slab_free(void *ptr);

#endif
