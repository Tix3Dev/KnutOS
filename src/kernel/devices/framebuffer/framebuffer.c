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

// draw a line to the framebuffer with start coordinates and end coordinates
void framebuffer_draw_line(int x_start_pos, int y_start_pos, int x_end_pos, int y_end_pos, uint32_t color)
{
	int dx = x_end_pos - x_start_pos;
	int dy = y_end_pos - y_start_pos;

	int x = x_start_pos;
	int y = y_start_pos;

	int p = 2 * dy - dx;

	while (x < x_end_pos)
	{
		if (p >= 0)
		{
			framebuffer_draw_pixel(x, y, color);
			y += 1;
			p = p + 2 * dy - 2 * dx;
		}
		else
		{
			framebuffer_draw_pixel(x, y, color);
			p = p + 2 * dy;
		}

		x += 1;
	}
}

// we divide the circle in 8 parts
void draw_circle_helper(int xc, int yc, int x, int y, uint32_t color)
{
	framebuffer_draw_pixel(xc + x, yc + y, color);
	framebuffer_draw_pixel(xc - x, yc + y, color);
	framebuffer_draw_pixel(xc + x, yc - y, color);
	framebuffer_draw_pixel(xc - x, yc - y, color);
	framebuffer_draw_pixel(xc + y, yc + x, color);
	framebuffer_draw_pixel(xc - y, yc + x, color);
	framebuffer_draw_pixel(xc + y, yc - x, color);
	framebuffer_draw_pixel(xc - y, yc - x, color);
}

// draw a circle to the framebuffer with the center of the circle and the radius
void framebuffer_draw_circle(int x_center, int y_center, int radius, uint32_t color)
{
	int x = 0;
	int y = radius;
	int d = 3 - 2 * radius;

	draw_circle_helper(x_center, y_center, x, y, color);

	while (y >= x)
	{
		x += 1;

		if (d > 0)
		{
			y -= 1;
			d = d + 4 * (x - y) + 10;
		}
		else
			d = d + 4 * x + 6;

		draw_circle_helper(x_center, y_center, x, y, color);
	}
}
