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
#include <stdint.h>

#include <boot/stivale2.h>
#include <libk/debug/debug.h>
#include <libk/kprintf/kprintf.h>
#include <libk/log/log.h>
#include <libk/stdio/stdio.h>

const char log_buffer[5120];

// variadic function for format specifiers
// serial logging - print log message to serial console
void serial_log_impl(char *description, int line_nr, STATUS status, char *fmt, ...)
{
    va_list ptr;
    va_start(ptr, fmt);
    vsnprintf((char *)&log_buffer, -1, fmt, ptr);

    if (status == INFO)
    {
        serial_set_color(TERM_CYAN);
        debug("[INFO]    | ");
    }
    else if (status == WARNING)
    {
        serial_set_color(TERM_YELLOW);
        debug("[WARNING] | ");
    }
    else if (status == ERROR)
    {
        serial_set_color(TERM_RED);
        debug("[ERROR]   | ");
    }

    debug("%s:%d ─→ %s", description, line_nr, (char *)log_buffer);
    serial_set_color(TERM_COLOR_RESET);
}

// variadic function for format specifiers
// kernel logging - print log message to framebuffer
void kernel_log_impl(char *description, int line_nr, STATUS status, char *fmt, ...)
{
    va_list ptr;
    va_start(ptr, fmt);
    vsnprintf((char *)&log_buffer, -1, fmt, ptr);

    if (status == INFO)
        printk(GFX_CYAN, "[INFO]    | %s:%d ─→ %s", description, line_nr, (char *)log_buffer);
    else if (status == WARNING)
        printk(GFX_YELLOW, "[WARNING] | %s:%d ─→ %s", description, line_nr, (char *)log_buffer);
    else if (status == ERROR)
        printk(GFX_RED, "[ERROR]   | %s:%d ─→ %s", description, line_nr, (char *)log_buffer);
}
