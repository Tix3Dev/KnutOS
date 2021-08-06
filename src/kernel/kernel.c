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

#include <stddef.h>
#include <stdint.h>

#include "boot/stivale2.h"
#include "boot/stivale2_boot.h"
#include "devices/framebuffer/framebuffer.h"
#include "devices/serial/serial.h"
#include "../libs/libk/debug/debug.h"

void kmain(struct stivale2_struct *stivale2_struct)
{
	serial_init();
	framebuffer_init(stivale2_struct, 0xFF754303);

	debug("Hello %s!\n", "World");

	serial_set_color(TERM_RED);
	serial_send_string("Hello World!\n");
	serial_set_color(TERM_COLOR_RESET);

	for (int x = 0; x < 100; x++)
	{
		for (int y = 0; y < 100; y++)
			framebuffer_draw_pixel(x, y, 0xFF29202C);
	}

	framebuffer_draw_line(100, 100, 200, 200, 0xFF29202C);

	framebuffer_draw_circle(200, 200, 143, 0xFF29202C);

	serial_set_color(TERM_CYAN);
	serial_send_string("Press any key to change screen...");
	serial_set_color(TERM_COLOR_RESET);
	serial_recv();
	framebuffer_reset_screen();

	framebuffer_print_char('!', 0, 0, 0xFF29202C);
	framebuffer_print_string("Hello World!\n", 0xFF29202C);
	framebuffer_print_string("!HELLO\tWORLD!\n\n", 0xFF29202C);

	for (;;)
	{
		framebuffer_print_string("hello\t", 0xFF29202C);

		for (int i = 0; i < 10000000; i++) {}
	}

	serial_send_string("\n");
	for (;;)
		serial_send_char(serial_recv());


	for (;;)
		asm ("hlt");
}
