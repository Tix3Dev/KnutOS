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

#ifndef PIC_H
#define PIC_H

#define PIC1			0x20		// I/O base address for master PIC
#define PIC2			0xA0		// I/O base address for slave PIC
#define PIC1_COMMAND	PIC1		// master PIC command port
#define PIC1_DATA		(PIC1+1)	// master PIC data port
#define PIC2_COMMAND	PIC2		// slave PIC command port
#define PIC2_DATA		(PIC2+1)	// slave PIC data port

void pic_remap(void);
void pic_signal_EOI(uint64_t isr_number);

#endif