/*
	BSD 3-Clause License

	Copyright (c) 2022, V01D
	All rights reserved.

	https://github.com/V01D-NULL/3ds-firmware/blob/main/src/arm11/core/mm/slab.c
*/

/*
	The first version was developed by Yves Vollmeier and Tim Thompson (in collaboration)
       	and the code can be seen here:
	https://github.com/V01D-NULL/slab_allocator
	This version was first developed by Tim Thompson and modified by
	Yves Vollmeier. Copyright header at the top is just for sanity.
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
