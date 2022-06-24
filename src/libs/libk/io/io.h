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

#ifndef IO_H
#define IO_H

void io_outb(uint16_t port, uint8_t value);
uint8_t io_inb(uint16_t port);
void io_wait(void);

#define FLAT_PTR(PTR) (*((uintptr_t(*)[])(PTR)))
#define BYTE_PTR(PTR) (*((uint8_t *)(PTR)))
#define WORD_PTR(PTR) (*((uint16_t *)(PTR)))
#define DWORD_PTR(PTR) (*((uint32_t *)(PTR)))
#define QWORD_PTR(PTR) (*((uint64_t *)(PTR)))

static inline void mmoutb(void *addr, uint8_t value) {
	asm volatile("mov %0, %1\n\t"
				 : "=m"(BYTE_PTR(addr))
				 : "r"(value)
				 : "memory");
}

static inline void mmoutw(void *addr, uint16_t value) {
	asm volatile("mov %0, %1\n\t"
				 : "=m"(WORD_PTR(addr))
				 : "r"(value)
				 : "memory");
}

static inline void mmoutd(void *addr, uint32_t value) {
	asm volatile("mov %0, %1\n\t"
				 : "=m"(DWORD_PTR(addr))
				 : "r"(value)
				 : "memory");
}

static inline void mmoutq(void *addr, uint64_t value) {
	asm volatile("mov %0, %1\n\t"
				 : "=m"(QWORD_PTR(addr))
				 : "r"(value)
				 : "memory");
}

static inline uint8_t mminb(void *addr) {
	uint8_t ret;
	asm volatile("mov %0, %1\n\t" : "=r"(ret) : "m"(BYTE_PTR(addr)) : "memory");
	return ret;
}

static inline uint16_t mminw(void *addr) {
	uint16_t ret;
	asm volatile("mov %0, %1\n\t" : "=r"(ret) : "m"(WORD_PTR(addr)) : "memory");
	return ret;
}

static inline uint32_t mmind(void *addr) {
	uint32_t ret;
	asm volatile("mov %0, %1\n\t"
				 : "=r"(ret)
				 : "m"(DWORD_PTR(addr))
				 : "memory");
	return ret;
}

static inline uint64_t mminq(void *addr) {
	uint64_t ret;
	asm volatile("mov %0, %1\n\t"
				 : "=r"(ret)
				 : "m"(QWORD_PTR(addr))
				 : "memory");
	return ret;
}

#endif
