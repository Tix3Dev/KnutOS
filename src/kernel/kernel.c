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

#include <stdint.h>
#include <stddef.h>

#include "boot/stivale2.h"
#include "boot/stivale2_boot.h"
#include "devices/serial/serial.h"
#include "../libs/libk/debug/debug.h"

void kmain(struct stivale2_struct *stivale2_struct)
{
	serial_init();

	debug("Hello %s!\n", "World");

	serial_set_color(TERM_RED);
	serial_send_string("Hello World!\n");
	serial_set_color(TERM_COLOR_RESET);

	for (;;)
		serial_send(serial_recv());


	for (;;)
		asm ("hlt");
}
