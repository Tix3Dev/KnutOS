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

void kmain(struct stivale2_struct *stivale2_struct)
{
	struct stivale2_struct_tag_terminal *term_str_tag;
	term_str_tag = stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_TERMINAL_ID);

	if (term_str_tag == NULL)
	{
		for (;;)
			asm ("hlt");
	}

	void *term_write_ptr = (void *)term_str_tag->term_write;

	void (*term_write)(const char *string, size_t length) = term_write_ptr;

	term_write("Hello World", 11);

	if (init_serial() == 0)
	{
		serial_send_string("Hello World");
		for (;;)
			serial_send(serial_recv());
	}
	// otherwise serial is faulty

	for (;;)
		asm ("hlt");
}
