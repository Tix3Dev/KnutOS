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

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#define GFX_BLACK	0xFF3B4252
#define GFX_RED		0xFFBF616A
#define GFX_GREEN	0xFFA3BE8C
#define GFX_YELLOW	0xFFEBCA8B
#define GFX_BLUE	0xFF81A1C1
#define GFX_PURPLE	0xFFB48EAD
#define GFX_CYAN	0xFF88C0D0
#define GFX_WHITE	0xFFE5E9F0

struct GFX_Struct
{
	uint64_t fb_addr;
	uint16_t fb_width;
	uint16_t fb_height;
	uint16_t fb_pitch;
	uint16_t fb_bpp;

	int glyph_width;
	int glyph_height;
};

void framebuffer_init(struct stivale2_struct *stivale2_struct, uint32_t background_color);
void framebuffer_draw_pixel(int x, int y, uint32_t color);
void framebuffer_set_background_color(uint32_t background_color);
void framebuffer_reset_screen(void);
void framebuffer_draw_line(int x_start_pos, int y_start_pos, int x_end_pos, int y_end_pos, uint32_t color);
void framebuffer_draw_circle(int x_center, int y_center, int radius, uint32_t color);
void framebuffer_move_one_row_up(void);
void framebuffer_print_char(uint32_t unicode_character, int x, int y, uint32_t foreground_color);
void framebuffer_print_string(char *string, uint32_t foreground_color);

#endif
