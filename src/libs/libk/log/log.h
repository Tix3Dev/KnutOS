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

#ifndef LOG_H
#define LOG_H

#define serial_log(...) serial_log_impl(__FILE__, __LINE__, __VA_ARGS__);
#define kernel_log(...) kernel_log_impl(__FILE__, __LINE__, __VA_ARGS__);

typedef enum {
    INFO,
    WARNING,
    ERROR
} STATUS;

void serial_log_impl(char *file, int line_nr, STATUS status, char *fmt, ...);
void kernel_log_impl(char *file, int line_nr, STATUS status, char *fmt, ...);

#endif
