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

#include <devices/apic/apic.h>
#include <devices/cpu/cpu.h>
#include <libk/debug/debug.h>
#include <libk/log/log.h>

void apic_init(void)
{
    if (!apic_is_available())
    {
        serial_log(ERROR, "No APIC was found on this computer!\n");
        kernel_log(ERROR, "No APIC was found on this computer!\n");


        serial_log(ERROR, "Kernel halted!\n");
        kernel_log(ERROR, "Kernel halted!\n");

        for (;;)
            asm ("hlt");
    }

    // remap pic 8259
    // mask all (maybe this could be a pic function)
    // -> maybe even combine both into one function, mhmm
}

bool apic_is_available(void)
{
    cpuid_registers_t *regs = &(cpuid_registers_t)
    {
        .leaf = CPUID_GET_FEATURES,
        .subleaf = 0,

        .eax = 0,
        .ebx = 0,
        .ecx = 0,
        .edx = 0
    };

    cpuid(regs);

    if (regs->edx & CPUID_FEAT_EDX_APIC)
	return true;

    return false;
}

void apic_enable(void)
{
    //
}

void apic_signal_EOI(void)
{
    // write to register with offset 0xB0 with the value 0
    // for that i am probably going to need a write function
}
