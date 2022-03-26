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

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef APIC_H
#define APIC_H

#define APIC_SPURIOUS_VECTOR_REGISTER	0xF
#define APIC_EOI_REGISTER		0xB0
#define APIC_SOFTWARE_ENABLE		(1 << 8)

void apic_init(void);
bool apic_is_available(void);
uint32_t lapic_read_register(uint32_t reg);
void lapic_write_register(uint32_t reg, uint32_t data);
void lapic_enable(void);
void lapic_signal_eoi(void);
void lapic_send_ipi(void);
uint32_t io_apic_read_register(size_t io_apic_i, uint8_t reg_offset);
void io_apic_write_register(size_t io_apic_i, uint8_t reg_offset, uint32_t data);

#endif
