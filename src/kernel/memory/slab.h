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

#include <stdbool.h>

#ifndef SLAB_H
#define SLAB_H

#define OBJECTS_PER_SLAB 5

typedef struct slab_cache   slab_cache_t;
typedef struct slab_state   slab_state_layer_t;
typedef struct slab	    slab_t;
typedef struct slab_object  slab_object_t;

// represents an object of a given slab
struct slab_object
{
    void *mem;

    bool is_allocated;
};

// represents a slab itself
struct slab
{
    slab_object_t objects[OBJECTS_PER_SLAB];
    struct slab *next;
};

// represents a slab state; there are three different ones used: free, used and partial
struct slab_state_layer
{
    bool is_full;
    bool is_empty;

    slab_t *head;

    struct slab_state *prev;
    struct slab_state *next;
};

// represents a (slab) cache
struct slab_cache
{
    uint32_t size; // size of every object in this cache

    uint64_t active_slab_count;	// number of slabs which are currently being used i.e. active
    uint64_t slab_create_count;	// number of slabs which have been created so far
    uint64_t slab_alloc_count;	// number of slabs which have been allocated so far
    uint64_t slab_free_count;	// number of slabs which have been freed so far
    
    struct slab_cache *prev;
    struct slab_cache *next;

    struct slab_state_layer *free    __attribute__((aligned(16)));
    struct slab_state_layer *used    __attribute__((aligned(16)));
    struct slab_state_layer *partial __attribute__((aligned(16)));
}; 

/* core functions */
void slab_init(void);
void slab_destroy(slab_cache_t *cache);
slab_cache_t *slab_create_cache(size_t size);
void *slab_alloc(size_t size);
bool slab_free(void *ptr);

/* public utility functions */
void slab_traverse_cache(slab_cache_t *cache);
void slab_print_slabs(slab_state_layer_t *type);
void slab_print_caches(void);

#endif
