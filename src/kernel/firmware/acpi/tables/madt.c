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

#include <boot/stivale2.h>
#include <boot/stivale2_boot.h>
#include <firmware/acpi/tables/madt.h>
#include <firmware/acpi/acpi.h>
#include <libk/debug/debug.h>
#include <libk/log/log.h>

madt_structure_t *madt;

void madt_init(void)
{
    madt = (madt_structure_t *)(uintptr_t)acpi_find_sdt_table("APIC");

    if (madt == NULL)
    {
        serial_log(ERROR, "No MADT was found on this computer!\n");
        kernel_log(ERROR, "No MADT was found on this computer!\n");


        serial_log(ERROR, "Kernel halted!\n");
        kernel_log(ERROR, "Kernel halted!\n");

        for (;;)
            asm ("hlt");
    }

    // iterate through ACPI table pointers

    size_t madt_table_length = (size_t)&madt->header + madt->header.length;

    uint8_t *table_ptr = (uint8_t *)&madt->table;

    while ((size_t)table_ptr < madt_table_length)
    {
	switch (*table_ptr)
	{
	    case PROCESSOR_LOCAL_APIC:
		serial_log(INFO, "MADT INIT: Found local APIC\n");
		kernel_log(INFO, "MADT INIT: Found local APIC\n");

		break;
	    case IO_APIC:
		serial_log(INFO, "MADT INIT: Found IO APIC\n");
		kernel_log(INFO, "MADT INIT: Found IO APIC\n");

		break;
	    case INTERRUPT_SOURCE_OVERRIDE:
		serial_log(INFO, "MADT INIT: Found interrupt source override\n");
		kernel_log(INFO, "MADT INIT: Found interrupt source override\n");

		break;
	    case LAPIC_NMI:
		serial_log(INFO, "MADT INIT: Found local APIC non maskable interrupt\n");
		kernel_log(INFO, "MADT INIT: Found local APIC non maskable interrupt\n");

		break;
	    case LAPIC_ADDRESS_OVERRIDE:
		serial_log(INFO, "MADT INIT: Found local APIC address override\n");
		kernel_log(INFO, "MADT INIT: Found local APIC address override\n");

		break;
	    case PROCESSOR_LOCAL_x2APIC:
		serial_log(INFO, "MADT INIT: Found processor local x2APIC\n");
		kernel_log(INFO, "MADT INIT: Found processor local x2APIC\n");

		break;
	}

	table_ptr += *(table_ptr + 1);
    }
}
