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

#include <libk/io/io.h>

void pit_init(uint32_t hz_frequency)
{
	uint32_t divisor = 1193182 / hz_frequency; // start frequency / target frequency
	io_outb(0x43, 0x36);
	io_outb(0x40, divisor & 0xFF);
	io_outb(0x40, (divisor >> 8) & 0xFF);
}

uint32_t pit_read_counter(void)
{
	uint32_t counter = 0;
	io_outb(0x43, 0x00);

	counter =	io_inb(0x40);
	counter |=	io_inb(0x40) << 8;
 
	return counter;
}

void pit_wait_ms(uint16_t ms)
{
	io_outb(0x43, 0x30);
	io_outb(0x40, ms & 0xFF);
	io_outb(0x40, (ms >> 8) & 0xFF);

	while (pit_read_counter() == 0) {}
}
