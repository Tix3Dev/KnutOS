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

#ifndef CPU_H
#define CPU_H

typedef struct
{
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    
    uint64_t isr_number;
    uint64_t error_code;
    
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} interrupt_cpu_state_t;

typedef struct
{
    uint32_t leaf;
    uint32_t subleaf;

    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
} cpuid_registers_t;

// https://github.com/qword-os/qword/blob/2e7899093d597dc55d8ed0d101f5c0509293d62f/src/sys/cpu.h#L67
// leaf is equivalent to a CPU function (same for subleaf, just 'sub-function')
static inline int cpuid(cpuid_registers_t *registers)
{
    uint32_t cpuid_max;

    asm volatile("cpuid"
	    : "=a" (cpuid_max)
	    : "a" (registers->leaf & 0x80000000)
	    : "rbx", "rcx", "rdx");

    if (registers->leaf > cpuid_max)
	return 0;

    asm volatile("cpuid"
	    : "=a" (registers->eax), "=b" (registers->ebx), "=c" (registers->ecx), "=d" (registers->edx)
	    : "a" (registers->leaf), "c" (registers->subleaf));

    return 1;
}

#endif
