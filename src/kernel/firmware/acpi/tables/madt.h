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

#include <firmware/acpi/tables/sdth.h>

#ifndef MADT_H
#define MADT_H

typedef struct __attribute__((__packed__))
{
    uint8_t entry_type; // according to madt_entry_type_t
    uint8_t record_length;
} madt_record_table_entry_t;

// uncommented the ones I probably won't be using for a long time
// -> because of that I didn't add the tables for them
typedef enum
{
    PROCESSOR_LOCAL_APIC = 0x0,
    IO_APIC = 0x1,
    INTERRUPT_SOURCE_OVERRIDE = 0x2,
    // NMI_SOURCE = 0x3,
    LAPIC_NMI = 0x4,
    // LAPIC_ADDRESS_OVERRIDE = 0x5,
    // IO_SAPIC = 0x6,
    // LOCAL_SAPIC = 0x7,
    // PLATFORM_INTERRUPT_SOURCES = 0x8,
    // PROCESSOR_LOCAL_x2APIC = 0x9
    // LOCAL_x2APIC_NMI = 0xA,
    // GIC_CPU_INTERFACE = 0xB,
    // GIC_DISTRIBUTOR = 0xC,
    // GIC_MSI_FRAME = 0xD
    // GIC_REDISTRIBUTOR = 0xE,
    // GIC_INTERRUPT_TRANSLATION_SERVICE = 0xF,
    // MULTIPROCESSOR_WAKEUP = 0x10
} madt_entry_type_t;

// entry type 0x0 - processor local apic
typedef struct __attribute__((__packed__))
{
    madt_record_table_entry_t record;
    uint8_t acpi_processor_id;
    uint8_t acpi_id;
    uint32_t flags;
} madt_lapic_t;

// entry type 0x1 - IO apic
typedef struct __attribute__((__packed__))
{
    madt_record_table_entry_t record;
    uint8_t io_apic_id;
    uint8_t reserved;
    uint8_t io_apic_address;
    uint8_t global_system_interrupt_base;
} madt_io_apic_t;

// entry type 0x2 - interrupt source override
typedef struct __attribute__((__packed__))
{
    madt_record_table_entry_t record;
    uint8_t bus_source;
    uint8_t irq_source;
    uint32_t global_system_interrupt;
    uint16_t flags;
} madt_iso_t;

// entry type 0x4 - non maskable interrupts
typedef struct __attribute__((__packed__))
{
    madt_record_table_entry_t record;
    uint8_t acpi_processor_id;
    uint16_t flags;
    uint8_t lint;
} madt_lapic_nmi_t;

typedef struct __attribute__((__packed__))
{
    sdt_header_t header;
    uint32_t lapic_address;
    uint32_t flags;
    madt_record_table_entry_t table[];
} madt_structure_t;

extern madt_structure_t *madt;

extern madt_lapic_t		    **madt_lapics;
extern madt_io_apic_t		    **madt_io_apics;
extern madt_iso_t		    **madt_isos;
extern madt_lapic_nmi_t	    **madt_lapic_nmis;

extern size_t madt_lapics_i;
extern size_t madt_io_apics_i;
extern size_t madt_isos_i;
extern size_t madt_lapic_nmis_i;

void madt_init(void);

#endif
