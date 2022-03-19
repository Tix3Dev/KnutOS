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

#include <devices/pic/pic.h>
#include <libk/io/io.h>

// disable PIC in order to use LAPIC and IOAPIC
void pic_disable(void)
{
    io_outb(PIC2_DATA, 0xFF);
    io_outb(PIC1_DATA, 0xFF);
}

// remap the programmable interrupt controller
void pic_remap(void)
{
    uint8_t mask1 = io_inb(PIC1_DATA);
    uint8_t mask2 = io_inb(PIC2_DATA);


    io_outb(PIC1_COMMAND, 0x11);
    io_outb(PIC2_COMMAND, 0x11);
    io_wait();


    io_outb(PIC1_DATA, 0x20);
    io_outb(PIC2_DATA, 0x28);
    io_wait();

    io_outb(PIC1_DATA, 0x04);
    io_outb(PIC2_DATA, 0x02);
    io_wait();

    io_outb(PIC1_DATA, 0x01);
    io_outb(PIC2_DATA, 0x01);
    io_wait();

    io_outb(PIC1_DATA, 0x00);
    io_outb(PIC2_DATA, 0x00);
    io_wait();


    io_outb(PIC1_DATA, ICW4_8086);
    io_wait();
    io_outb(PIC2_DATA, ICW4_8086);
    io_wait();


    io_outb(PIC1_DATA, mask1);
    io_outb(PIC2_DATA, mask2);
}

// use internel PIC register: IMR to ignore irq_lines
void pic_set_mask(uint8_t irq_line)
{
    uint16_t port;
    uint8_t value;

    if (irq_line < 8)
        port = PIC1_DATA;

    else
    {
        port = PIC2_DATA;
        irq_line -= 8;
    }

    value = io_inb(port) | (1 << irq_line);
    io_outb(port, value);
}

// "undo" pic_set_mask
void pic_clear_mask(uint8_t irq_line)
{
    uint16_t port;
    uint8_t value;

    if (irq_line < 8)
        port = PIC1_DATA;

    else
    {
        port = PIC2_DATA;
        irq_line -= 8;
    }

    value = io_inb(port) & ~(1 << irq_line);
    io_outb(port, value);
}

// signal an end of interrupt
void pic_signal_EOI(uint64_t isr_number)
{
    if (isr_number >= 40)				// if the IRQ came from the slave PIC
        io_outb(PIC2_COMMAND, 0x20);

    io_outb(PIC1_COMMAND, 0x20);		// if the IRQ came from the master and/or the slave PIC
}
