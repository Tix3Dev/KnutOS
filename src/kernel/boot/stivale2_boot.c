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

#include <boot/stivale2.h>

static uint8_t stack[4096];


static struct stivale2_tag la57_tag =
{
    .identifier	= STIVALE2_HEADER_TAG_5LV_PAGING_ID,
    .next	= 0
};

static struct stivale2_header_tag_framebuffer framebuffer_hdr_tag =
{
    .tag =
    {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next	    = (uintptr_t)&la57_tag
    },
    // bootloader automatically finds best values
    .framebuffer_width	= 0,
    .framebuffer_height	= 0,
    .framebuffer_bpp	= 0
};

__attribute__((section(".stivale2hdr"), used))
static struct stivale2_header stivale_hdr =
{
    .entry_point    = 0,
    .stack	    = (uintptr_t)stack + sizeof(stack),
    .flags	    = (1 << 1) | (1 << 2),
    .tags	    = (uintptr_t)&framebuffer_hdr_tag
};

void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, uint64_t id)
{
    struct stivale2_tag *current_tag = (void *)stivale2_struct->tags;

    for (;;)
    {
        if (current_tag == NULL)
            return NULL;

        if (current_tag->identifier == id)
            return current_tag;

        current_tag = (void *)current_tag->next;
    }
}
