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

#include <stddef.h>

#include <devices/apic/apic.h>
#include <devices/cpu/cpu.h>
#include <devices/pic/pic.h>
#include <firmware/acpi/tables/madt.h>
#include <interrupts/interrupts.h>
#include <memory/mem.h>
#include <libk/debug/debug.h>
#include <libk/log/log.h>

uintptr_t lapic_base;

/* General APIC functions */

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

    lapic_base = phys_to_higher_half_data((uintptr_t)madt->lapic_address);

    pic_remap();
    pic_disable();

    lapic_enable();
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

/* LAPIC functions */

// returns the value of a local apic register
// currently not laid out for x2apic, otherwise would check
// if supported and then use msr
uint32_t lapic_read_register(uint32_t reg)
{
    return *((volatile uint32_t *)lapic_base + reg);
}

// write to a local apic manager
// currently not laid out for x2apic, otherwise would check
// if supported and then use msr
void lapic_write_register(uint32_t reg, uint32_t data)
{
    *((volatile uint32_t *)(lapic_base + reg)) = data;
}

// enable lapic to make it receive interrupts
void lapic_enable(void)
{
    lapic_write_register(APIC_SPURIOUS_VECTOR_REGISTER, APIC_SOFTWARE_ENABLE | SPURIOUS_INTERRUPT);
}

// signal an end of interrupt to continue execution
void lapic_signal_eoi(void)
{
    lapic_write_register(APIC_EOI_REGISTER, 0);
}

void lapic_send_ipi(void)
{
    // send a raw inter processor interrupt to a specific APIC ID
}

/* IO APIC functions */

// returns the value of a ioapic register
uint32_t io_apic_read_register(size_t io_apic_i, uint8_t reg_offset)
{
    uint32_t volatile *current_io_apic_base = (uint32_t volatile *)madt_io_apics[io_apic_i];

    // IOREGSEL
    *current_io_apic_base = reg_offset;

    // IOWIN
    return *(current_io_apic_base + 0x10);
}

void io_apic_write_register(size_t io_apic_i, uint8_t reg_offset, uint32_t data)
{
    uint32_t volatile *current_io_apic_base = (uint32_t volatile *)madt_io_apics[io_apic_i];

    // IOREGSEL
    *current_io_apic_base = reg_offset;

    // IOWIN
    *(current_io_apic_base + 0x10) = data;
}
