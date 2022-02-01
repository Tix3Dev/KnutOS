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

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <boot/stivale2.h>
#include <boot/stivale2_boot.h>
#include <memory/bump.h>
#include <memory/slab.h>
#include <libk/math/math.h>

static slab_t slabs[SLAB_COUNT];

/* utility functions */

static int32_t find_free_object(int32_t slab_index);
static int32_t find_allocated_object(int32_t slab_index);

/* core functions */

void slab_init(struct stivale2_struct *stivale2_struct)
{
    for (int32_t i = 0; i < SLAB_COUNT; i++)
    {
	slabs[i].size = pow(2, i + 1);
	slabs[i].is_full = false;

	int32_t objects_per_slab = pow(2, SLAB_COUNT) / pow(2, i + 1);
	for (int j = 0; j < objects_per_slab; j++)
	    slabs[i].objects[j] = bump_alloc(stivale2_struct, slabs[i].size);
    }
}

void *slab_alloc(size_t size)
{
    // TODO: if is_full create new default cache

    void *return_value = NULL;

    for (int32_t i = 0; i < SLAB_COUNT; i++)
    {
	if (slabs[i].size != size)
	    continue;

	if (slabs[i].is_full)
	    return return_value;

	int32_t free_object_index = find_free_object(i);

	if (free_object_index == -1)
	    slabs[i].is_full = true;
	else
	{
	    return_value = slabs[i].objects[free_object_index];
	    slabs[i].objects[free_object_index] = NULL;
	}

	return return_value;
    }

    return return_value;
}

void slab_free(void *ptr, size_t size)
{
    //
}

/* utility functions */

static int32_t find_free_object(int32_t slab_index)
{
    int32_t objects_per_slab = pow(2, SLAB_COUNT) / pow(2, slab_index + 1);

    for (int32_t i = 0; i < objects_per_slab; i++)
	if (slabs[slab_index].objects[i] != NULL)
	    return i;

    return -1;
}

static int32_t find_allocated_object(int32_t slab_index)
{
    //
}
