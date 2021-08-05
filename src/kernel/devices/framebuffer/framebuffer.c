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

#include <stdarg.h>
#include <stddef.h>

#include "../../boot/stivale2.h"
#include "../../boot/stivale2_boot.h"
#include "framebuffer.h"

struct GFX_Struct
{
	struct stivale2_struct_tag_framebuffer *framebuffer_info;

	uint32_t global_background_color;

	int cursor_x_pos;
	int cursor_y_pos;
} gfx;

void framebuffer_init(struct stivale2_struct *stivale2_struct)
{
    struct stivale2_struct_tag_framebuffer *framebuffer_info = stivale2_get_tag(stivale2_struct,
																				STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);

	gfx.framebuffer_info = framebuffer_info;

	// default background color is black; when no custom background color was set the screen reset will still work with this
	gfx.global_background_color = 0xFF000000;

	gfx.cursor_x_pos = 0;
	gfx.cursor_y_pos = 0;
}

void framebuffer_draw_pixel(int x, int y, uint32_t color)
{
	size_t fb_index = y * (gfx.framebuffer_info->framebuffer_pitch / sizeof(uint32_t)) + x;
	uint32_t *fb = (uint32_t *)gfx.framebuffer_info->framebuffer_addr;
	
	fb[fb_index] = color;
}

void framebuffer_set_background_color(uint32_t background_color)
{
	gfx.global_background_color = background_color;

	for (int y = 0; y < gfx.framebuffer_info->framebuffer_height; y++)
	{
		for (int x = 0; x < gfx.framebuffer_info->framebuffer_width; x++)
			framebuffer_draw_pixel(x, y, gfx.global_background_color);
	}
}

void framebuffer_reset_screen(void)
{
	gfx.cursor_x_pos = 0;
	gfx.cursor_y_pos = 0;

	framebuffer_set_background_color(gfx.global_background_color);
}
