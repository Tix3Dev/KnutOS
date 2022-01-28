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

#include <stdint.h>

// #include <boot/stivale2.h>
#include <devices/serial/serial.h>
#include <libk/io/io.h>
// #include <libk/log/log.h>
// #include <libk/stdio/stdio.h>

// set COM1 offsets to init value
void serial_init(void)
{
    io_outb(COM1 + 1, 0x00);
    io_outb(COM1 + 3, 0x80);
    io_outb(COM1 + 0, 0x03);
    io_outb(COM1 + 1, 0x00);
    io_outb(COM1 + 3, 0x03);
    io_outb(COM1 + 2, 0xC7);
    io_outb(COM1 + 4, 0x0B);

    // serial_log(INFO, "Serial initialized\n");
    // kernel_log(INFO, "Serial initialized\n");
}

// check if transmission buffer is not empty
int is_serial_received(void)
{
    return io_inb(COM1 + 5) & 1;
}

// read data from COM1
char serial_recv(void)
{
    while (is_serial_received() == 0);

    return io_inb(COM1);
}

// check if transmission buffer is empty
int is_transmit_empty(void)
{
    return io_inb(COM1 + 5) & 0x20;
}

// send data to COM1
void serial_send_char(char c)
{
    while (is_transmit_empty() == 0);

    io_outb(COM1, c);
}

// send even more data to COM1
void serial_send_string(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
        serial_send_char(str[i]);
}

// send bash color codes to COM1
void serial_set_color(char *color_code)
{
    if (color_code[0] != '\e' || color_code[1] != '[')
        return;

    serial_send_string(color_code);
}
