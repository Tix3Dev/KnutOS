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
#include <stdint.h>

#include "../../../kernel/boot/stivale2.h"
#include "../../../kernel/devices/framebuffer/framebuffer.h"
#include "../kprintf/kprintf.h"

const char printk_buffer[512];

// variadic function for format specifiers to print to the framebuffer
void printk(uint32_t foreground_color, char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vsnprintf((char*)&printk_buffer, -1, fmt, ap);

	framebuffer_print_string((char*)printk_buffer, foreground_color);

	va_end(ap);
}
