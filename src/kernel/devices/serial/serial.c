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

#include "serial.h"
#include "../../../libs/libk/io.h"

int init_serial()
{
	io_outb(COM1 + 1, 0x00);
	io_outb(COM1 + 3, 0x80);
	io_outb(COM1 + 0, 0x03);
	io_outb(COM1 + 1, 0x00);
	io_outb(COM1 + 3, 0x03);
	io_outb(COM1 + 2, 0xC7);
	io_outb(COM1 + 4, 0x0B);
	io_outb(COM1 + 4, 0x1E);	// set it in loopback mode -> our testmode

	io_outb(COM1 + 0, 0xAE);	// only for check -> serial is faulty if the byte send is not the same as the byte returned

	if (io_inb(COM1 + 0) != 0xAE)
		return 1;

	io_outb(COM1 + 4, 0x0F);	// change from loopback mode to normal operation mode

	return 0;
}

int is_serial_received()
{
	return io_inb(COM1 + 5) & 1;
}

char serial_recv()
{
	while (is_serial_received() == 0);

	return io_inb(COM1);
}

int is_transmit_empty()
{
	return io_inb(COM1 + 5) & 0x20;
}

void serial_send(char c)
{
	while (is_transmit_empty() == 0);

	io_outb(COM1, c);
}

void serial_send_string(char* str)
{
	for (int i = 0; str[i] != '\0'; i++)
		serial_send(str[i]);
}
