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

#include <libk/debug/debug.h>
#include <libk/kprintf/kprintf.h>
#include <libk/log/log.h>

const char log_buffer[5120];

// variadic function for format specifiers
void log(STATUS status, char *description, char *fmt, ...)
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

	debug("%s ─→ %s", description, (char *)log_buffer);
	serial_set_color(TERM_COLOR_RESET);
}
