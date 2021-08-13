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

#include <devices/pic/pic.h>
#include <interrupts/idt.h>
#include <libk/io/io.h>

extern void			_load_idt_asm(struct IDT_Pointer *ptr);
extern uintptr_t	_isr_names_asm[];

static struct IDT_Descriptor	idt[256];
static struct IDT_Pointer		idt_pointer;

// set the arguments for a IDT entry
void create_descriptor(uint8_t index, uint8_t type_and_attributes)
{
	uint64_t offset = _isr_names_asm[index];

	idt[index].offset_15_0			= offset & 0xFFFF;
	idt[index].selector				= 0x08;
	idt[index].ist					= 0;
	idt[index].type_and_attributes	= type_and_attributes;
	idt[index].offset_31_16			= (offset >> 16) & 0xFFFF;
	idt[index].offset_63_32			= (offset >> 32) & 0xFFFFFFFF;
	idt[index].zero					= 0;
}

// create descriptors, remap the PIC and load IDT
void idt_init(void)
{
	// create 32 descriptors for exceptions
	create_descriptor(0, 0x8E);
	create_descriptor(1, 0x8E);
	create_descriptor(2, 0x8E);
	create_descriptor(3, 0x8E);
	create_descriptor(4, 0x8E);
	create_descriptor(5, 0x8E);
	create_descriptor(6, 0x8E);
	create_descriptor(7, 0x8E);
	create_descriptor(8, 0x8E);
	create_descriptor(9, 0x8E);
	create_descriptor(10, 0x8E);
	create_descriptor(11, 0x8E);
	create_descriptor(12, 0x8E);
	create_descriptor(13, 0x8E);
	create_descriptor(14, 0x8E);
	create_descriptor(15, 0x8E);
	create_descriptor(16, 0x8E);
	create_descriptor(17, 0x8E);
	create_descriptor(18, 0x8E);
	create_descriptor(19, 0x8E);
	create_descriptor(20, 0x8E);
	create_descriptor(21, 0x8E);
	create_descriptor(22, 0x8E);
	create_descriptor(23, 0x8E);
	create_descriptor(24, 0x8E);
	create_descriptor(25, 0x8E);
	create_descriptor(26, 0x8E);
	create_descriptor(27, 0x8E);
	create_descriptor(28, 0x8E);
	create_descriptor(29, 0x8E);
	create_descriptor(30, 0x8E);
	create_descriptor(31, 0x8E);

	// remap the PIC
	pic_remap();

	// create 16 descriptors IRQ (the 16 standard ISA IRQs)
	create_descriptor(32, 0x8E);
	create_descriptor(33, 0x8E);
	create_descriptor(34, 0x8E);
	create_descriptor(35, 0x8E);
	create_descriptor(36, 0x8E);
	create_descriptor(37, 0x8E);
	create_descriptor(38, 0x8E);
	create_descriptor(39, 0x8E);
	create_descriptor(40, 0x8E);
	create_descriptor(41, 0x8E);
	create_descriptor(42, 0x8E);
	create_descriptor(43, 0x8E);
	create_descriptor(44, 0x8E);
	create_descriptor(45, 0x8E);
	create_descriptor(46, 0x8E);
	create_descriptor(47, 0x8E);

	// load IDT into idtr register
	idt_pointer.limit	= sizeof(idt) - 1;
	idt_pointer.base	= (uint64_t)&idt;

	_load_idt_asm(&idt_pointer);
}
