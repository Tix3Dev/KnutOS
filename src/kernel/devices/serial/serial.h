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

#ifndef SERIAL_H
#define SERIAL_H

#define COM1 0x3f8

// bash color codes
#define TERM_BLACK	    "\e[0;30m"
#define TERM_RED	    "\e[0;31m"
#define TERM_GREEN	    "\e[0;32m"
#define TERM_YELLOW	    "\e[0;33m"
#define TERM_BLUE	    "\e[0;34m"
#define TERM_PURPLE	    "\e[0;35m"
#define TERM_CYAN	    "\e[0;36m"
#define TERM_WHITE	    "\e[0;37m"
#define TERM_COLOR_RESET    "\e[0m"

void serial_init(void);
char serial_recv(void);
void serial_send_char(char c);
void serial_send_string(char *str);
void serial_set_color(char *color_code);

#endif
