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
#include <stdint.h>
#include <stddef.h>

#include <boot/stivale2.h>
#include <boot/stivale2_boot.h>
#include <memory/bump.h>
#include <memory/slab.h>

// /* private utility functions */
// bool is_page_aligned(uint32_t num);
// bool is_power_of_two(uint32_t num);
// bool is_slab_empty(slab_t *slab);
// slab_t *create_slab(size_t size);
// void append_slab(slab_t **ref, slab_t *new_node);
// void append_to_global_cache(slab_cache_t **ref, slab_cache_t *new_cache);
// void remove_from_global_cache(slab_cache_t *cache);
// void remove_slab_head(slab_state_layer_t *state);
// slab_cache_t *get_previous_cache(slab_cache_t *cache);
//
// /* global linked list and pointer to it's head */
// static slab_cache_t *cache_list;
// static slab_cache_t *cache_list_head;
//
// /* core functions */
// void slab_init(void)
// {
//     cache_list = bump_alloc(NULL, sizeof(slab_cache_t));
//
//     // create size oriented caches which are commonly used
//     slab_create_cache(2);
//     slab_create_cache(4);
//     slab_create_cache(8);
//     slab_create_cache(16);
//     slab_create_cache(32);
//     slab_create_cache(64);
//     slab_create_cache(128);
//     slab_create_cache(256);
//     slab_create_cache(512);
// }
//
// void slab_destroy(slab_cache_t *cache)
// {
//     //
// }
//
// slab_cache_t *slab_create_cache(size_t size)
// {
//     //
// }
//
// void *slab_alloc(size_t size)
// {
//     //
// }
//
// bool slab_free(void *ptr)
// {
//     //
// }
//
// /* public utility functions */
// void slab_traverse_cache(slab_cache_t *cache)
// {
//     //
// }
//
// void slab_print_slabs(slab_state_layer_t *type)
// {
//     //
// }
//
// void slab_print_caches(void)
// {
//     //
// }
//
//
// /* private utility functions */
// bool is_page_aligned(uint32_t num)
// {
//     //
// }
//
// bool is_power_of_two(uint32_t num)
// {
//     //
// }
//
// bool is_slab_empty(slab_t *slab)
// {
//     //
// }
//
// slab_t *create_slab(size_t size)
// {
//     //
// }
//
// void append_slab(slab_t **ref, slab_t *new_node)
// {
//     //
// }
//
// void append_to_global_cache(slab_cache_t **ref, slab_cache_t *new_cache)
// {
//     //
// }
//
// void remove_from_global_cache(slab_cache_t *cache)
// {
//     //
// }
//
// void remove_slab_head(slab_state_layer_t *state)
// {
//     //
// }
//
// slab_cache_t *get_previous_cache(slab_cache_t *cache)
// {
//     //
// }
