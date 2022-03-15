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
#include <libk/alloc/kmalloc.h>
#include <libk/log/log.h>

madt_structure_t *madt;

madt_lapic_t		    **madt_lapics;
size_t madt_lapics_i	    = 0;

madt_io_apic_t		    **madt_io_apics;
size_t madt_io_apics_i	    = 0;

madt_iso_t		    **madt_isos;
size_t madt_isos_i	    = 0;

madt_lapic_nmi_t	    **madt_lapic_nmis;
size_t madt_lapic_nmis_i    = 0;

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


    madt_lapics	    = kmalloc(256);
    madt_io_apics   = kmalloc(256);
    madt_isos	    = kmalloc(256);
    madt_lapic_nmis = kmalloc(256);


    size_t madt_table_length = (size_t)&madt->header + madt->header.length;

    uint8_t *table_ptr = (uint8_t *)&madt->table;

    while ((size_t)table_ptr < madt_table_length)
    {
	switch (*table_ptr)
	{
	    case PROCESSOR_LOCAL_APIC:
		serial_log(INFO, "MADT Initialization: Found local APIC\n");
		kernel_log(INFO, "MADT Initialization: Found local APIC\n");

		madt_lapics[madt_lapics_i++] = (madt_lapic_t *)table_ptr;

		break;
	    case IO_APIC:
		serial_log(INFO, "MADT Initialization: Found IO APIC\n");
		kernel_log(INFO, "MADT Initialization: Found IO APIC\n");

		madt_io_apics[madt_io_apics_i++] = (madt_io_apic_t *)table_ptr;

		break;
	    case INTERRUPT_SOURCE_OVERRIDE:
		serial_log(INFO, "MADT Initialization: Found interrupt source override\n");
		kernel_log(INFO, "MADT Initialization: Found interrupt source override\n");

		madt_isos[madt_isos_i++] = (madt_iso_t *)table_ptr;

		break;
	    case LAPIC_NMI:
		serial_log(INFO, "MADT Initialization: Found local APIC non maskable interrupt\n");
		kernel_log(INFO, "MADT Initialization: Found local APIC non maskable interrupt\n");

		madt_lapic_nmis[madt_lapic_nmis_i++] = (madt_lapic_nmi_t *)table_ptr;

		break;
	}

	table_ptr += *(table_ptr + 1);
    }
}

madt_lapic_t *madt_get_lapics(void)
{
    return *madt_lapics;
}

madt_io_apic_t *madt_get_io_apics(void)
{
    return *madt_io_apics;
}

madt_iso_t *madt_get_isos(void)
{
    return *madt_isos;
}

madt_lapic_nmi_t *madt_get_lapic_nmis(void)
{
    return *madt_lapic_nmis;
}
