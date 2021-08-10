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

#ifndef GDT_H
#define GDT_H

// describes the attributes of a given segment
struct GDT_Descriptor
{
	uint16_t	limit_15_0;
	uint16_t	base_15_0;
	uint8_t		base_23_16;
	uint8_t		type;
	uint8_t		limit_19_16_and_flags;
	uint8_t		base_31_24;
} __attribute__((packed)); // for no special compiler optimization

// task state segment (for graphical layout see Intel SDM Figure 7-11)
struct TSS
{
    uint32_t	reserved0;
	uint64_t	rsp0;
	uint64_t	rsp1;
    uint64_t	rsp2;
	uint64_t	reserved1;
	uint64_t	ist1;
    uint64_t	ist2;
	uint64_t	ist3;
	uint64_t	ist4;
    uint64_t	ist5;
	uint64_t	ist6;
	uint64_t	ist7;
    uint64_t	reserved2;
	uint16_t	reserved3;
	uint16_t	iopb_offset;
} __attribute__((packed)); // for no special compiler optimization

// combines everything
__attribute__((aligned(4096)))
struct GDT
{
  struct GDT_Descriptor null;
  struct GDT_Descriptor kernel_code;
  struct GDT_Descriptor kernel_data;
  struct GDT_Descriptor null2;
  struct GDT_Descriptor user_data;
  struct GDT_Descriptor user_code;
  struct GDT_Descriptor ovmf_data;
  struct GDT_Descriptor ovmf_code;
  struct GDT_Descriptor tss_low;
  struct GDT_Descriptor tss_high;
} __attribute__((packed)); // for no special compiler optimization

// used to load the GDT into the gdtr register
struct GDT_Pointer
{
	uint16_t base;
	uint64_t limit;
} __attribute__((packed)); // for no special compiler optimization

void gdt_init(void);

#endif
