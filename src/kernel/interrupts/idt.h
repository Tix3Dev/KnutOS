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

#ifndef IDT_H
#define IDT_H

// describes the attributes of an IDT entry
struct IDT_Descriptor
{
	uint16_t	offset_15_0;			// offset bits 0..15
	uint16_t	selector;				// a code segment selector in GDT or LDT
	uint8_t		ist;      				// bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
	uint8_t		type_and_attributes;	// type and attributes
	uint16_t	offset_31_16;			// offset bits 16..31
	uint32_t	offset_63_32;			// offset bits 32..63
	uint32_t	zero;    				// reserved
}__attribute__((packed)); // for no special compiler optimization

// used to load the IDT into the idtr register
struct IDT_Pointer
{
	uint16_t limit;
	uint64_t base;
}__attribute__((packed)); // for no special compiler optimization

void idt_init(void);

#endif
