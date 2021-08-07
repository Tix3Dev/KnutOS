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
#include "logo.h"
#include "../libs/libk/debug/debug.h"
#include "../libs/libk/stdio/stdio.h"

void kmain(struct stivale2_struct *stivale2_struct)
{
	serial_init();
	framebuffer_init(stivale2_struct, 0xFF754303);

	debug("\e[0;35mWelcome to:\n");
	debug("\e[0;35m%s", small_logo_text);
	serial_set_color("\e[0m");

	printk(0xFF29202C, "Welcome to:\n\n");
	printk(0xFF29202C, "%s", big_logo);
	
	for (;;)
		asm ("hlt");
}
	
