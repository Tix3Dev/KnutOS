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

#include <stdarg.h>
#include <stddef.h>

#include <boot/stivale2.h>
#include <boot/stivale2_boot.h>
#include <devices/framebuffer/framebuffer.h>
// #include <libk/log/log.h>

#define SSFN_CONSOLEBITMAP_TRUECOLOR	// use the special renderer for 32 bit truecolor packed pixels
#define SSFN_NOIMPLEMENTATION		// dont' use the normal renderer implementation
#include <libk/ssfn.h>

// this name follows the SSFN rule: _binary_(filename)_start
// we also need to do a few things in the makefile:
// first we turn the .sfn font into an object file
// then we have to link it as any other object file
extern uint8_t _binary_sfn_fonts_unifont_sfn_start;

struct GFX_Struct gfx;

// save information about the framebuffer in a struct
// set the basic SSFN variables
// change the background color
void framebuffer_init(struct stivale2_struct *stivale2_struct, uint32_t background_color)
{
    struct stivale2_struct_tag_framebuffer *framebuffer_info = stivale2_get_tag(stivale2_struct,
            STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);

    gfx.fb_addr		= framebuffer_info->framebuffer_addr;
    gfx.fb_width	= framebuffer_info->framebuffer_width;
    gfx.fb_height	= framebuffer_info->framebuffer_height;
    gfx.fb_pitch	= framebuffer_info->framebuffer_pitch;
    gfx.fb_bpp		= framebuffer_info->framebuffer_bpp;
    gfx.glyph_width	= 8;
    gfx.glyph_height	= 16;

    ssfn_src	    = (ssfn_font_t *)&_binary_sfn_fonts_unifont_sfn_start;
    ssfn_dst.ptr    = (uint8_t *)gfx.fb_addr;
    ssfn_dst.w	    = gfx.fb_width;
    ssfn_dst.h	    = gfx.fb_height;
    ssfn_dst.p	    = gfx.fb_pitch;
    ssfn_dst.x	    = 0;
    ssfn_dst.y	    = 0;

    framebuffer_set_background_color(background_color);

    // serial_log(INFO,	"Framebuffer initialized\n");
    // kernel_log(INFO,	"Framebuffer initialized\n");
}

// draw one pixel at coordinate x, y (0, 0 is top left corner) in a certain color
void framebuffer_draw_pixel(int x, int y, uint32_t color)
{
    size_t fb_index = y * (gfx.fb_pitch / sizeof(uint32_t)) + x;
    uint32_t *fb = (uint32_t *)gfx.fb_addr;

    fb[fb_index] = color;
}

// set the 'global' background color and turn every pixel into that color
void framebuffer_set_background_color(uint32_t background_color)
{
    ssfn_dst.bg = background_color;

    for (int y = 0; y < gfx.fb_height; y++)
    {
        for (int x = 0; x < gfx.fb_width; x++)
            framebuffer_draw_pixel(x, y, ssfn_dst.bg);
    }
}

// reset pen position and turn every pixel into the default background color
void framebuffer_reset_screen(void)
{
    ssfn_dst.x = 0;
    ssfn_dst.y = 0;

    framebuffer_set_background_color(ssfn_dst.bg);
}

// draw a line to the framebuffer with start coordinates and end coordinates
// making use of bresenham's line algorithm
void framebuffer_draw_line(int x_start_pos, int y_start_pos, int x_end_pos, int y_end_pos, uint32_t color)
{
    int dx;
    int dy;
    int sx;
    int sy;
    int error1;
    int error2;

    if (x_start_pos == x_end_pos)
    {
        if (y_start_pos < y_end_pos)
            for (int y = y_start_pos; y <= y_end_pos; y++)
                framebuffer_draw_pixel(x_start_pos, y, color);
        else
            for (int y = y_end_pos; y <= y_start_pos; y++)
                framebuffer_draw_pixel(x_start_pos, y, color);

        return;
    }

    if (y_start_pos == y_end_pos)
    {
        if (x_start_pos < x_end_pos)
            for (int x = x_start_pos; x <= x_end_pos; x++)
                framebuffer_draw_pixel(x, y_start_pos, color);
        else
            for (int x = x_end_pos; x <= x_start_pos; x++)
                framebuffer_draw_pixel(x, y_start_pos, color);

        return;
    }

    if (x_start_pos < x_end_pos)
    {
        dx = x_end_pos - x_start_pos;
        sx = 1;
    }
    else
    {
        dx = x_start_pos - x_end_pos;
        sx = -1;
    }

    if (y_start_pos < y_end_pos)
    {
        dy = -(y_end_pos - y_start_pos);
        sy = 1;
    }
    else
    {
        dy = -(y_start_pos - y_end_pos);
        sy = -1;
    }

    error1 = dx + dy;

    framebuffer_draw_pixel(x_start_pos, y_start_pos, color);

    while (x_start_pos != x_end_pos && y_start_pos != y_end_pos)
    {
        error2 = 2 * error1;

        if (error2 >= dy)
        {
            error1 += dy;
            x_start_pos += sx;
        }

        if (error2 <= dx)
        {
            error1 += dx;
            y_start_pos += sy;
        }

        framebuffer_draw_pixel(x_start_pos, y_start_pos, color);
    }
}

// divide the circle into 8 parts
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

// draw a circle with the x, y coordinate being the center
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

// memmove the screen by one 'glyph height'
void framebuffer_move_one_row_up(void)
{
    uint32_t *fb = (uint32_t *)gfx.fb_addr;

    for (int column = gfx.glyph_height; column < gfx.fb_height; column++)
    {
        for (int row = 0; row < gfx.fb_width; row++)
        {
            size_t current_index = column * (gfx.fb_pitch / sizeof(uint32_t)) + row;
            uint32_t current_color = fb[current_index];
            fb[current_index] = ssfn_dst.bg;

            size_t new_index = (column - gfx.glyph_height) * (gfx.fb_pitch / sizeof(uint32_t)) + row;
            fb[new_index] = current_color;
        }
    }
}

// print a glyph to the screen in a certain color
void framebuffer_print_char(uint32_t unicode, int x, int y, uint32_t foreground_color)
{
    ssfn_dst.x = x;
    ssfn_dst.y = y;

    if (ssfn_dst.x + gfx.glyph_width > gfx.fb_width) // here too problematic
    {
        ssfn_dst.x = 0;
        ssfn_dst.y += gfx.glyph_height;
    }

    if (ssfn_dst.y >= gfx.fb_height) // this is problematic cuz don't do this when backspace
    {
        ssfn_dst.x = 0;
        ssfn_dst.y = gfx.fb_height - gfx.glyph_height;
        framebuffer_move_one_row_up();
    }

    // if the unicode character is a tab we print four spaces
    if (unicode == '\t')
    {
        for (int i = 0; i < 4; i++)
            framebuffer_print_string(" ", foreground_color);

        return;
    }

    // if the unicode character is a backspace we check where the cursor is and then
    // replace the last character with space and put the cursor there
    if (unicode == '\b')
    {
        if (ssfn_dst.x == 0)
        {
            if (ssfn_dst.y == 0)
                return;

            ssfn_dst.x = gfx.fb_width - gfx.glyph_width;
            ssfn_dst.y -= gfx.glyph_height;

            ssfn_putc(' ');

            ssfn_dst.x -= gfx.glyph_width;
        }
        else
        {
            ssfn_dst.x -= gfx.glyph_width;

            ssfn_putc(' ');

            ssfn_dst.x -= gfx.glyph_width;
        }

        return;
    }

    ssfn_dst.fg = foreground_color;

    ssfn_putc(unicode);
}

// print a string of glyphs to the screen in a certain color
void framebuffer_print_string(char *string, uint32_t foreground_color)
{
    while (*string)
        framebuffer_print_char(ssfn_utf8(&string), ssfn_dst.x, ssfn_dst.y, foreground_color);
}
