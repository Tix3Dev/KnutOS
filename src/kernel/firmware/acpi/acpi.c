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
#include <firmware/acpi/tables/rsdp.h>
#include <firmware/acpi/tables/rsdt.h>
#include <firmware/acpi/tables/sdth.h>
#include <firmware/acpi/acpi.h>
#include <libk/debug/debug.h>
#include <libk/log/log.h>
#include <libk/stdio/stdio.h>
#include <libk/string/string.h>

static rsdt_structure_t *rsdt;

void acpi_init(struct stivale2_struct *stivale2_struct)
{
    struct stivale2_struct_tag_rsdp *rsdp_tag = stivale2_get_tag(stivale2_struct,
	    STIVALE2_STRUCT_TAG_RSDP_ID);

    rsdp_init(rsdp_tag->rsdp);

    rsdt = (rsdt_structure_t *)(uintptr_t)get_rsdp_structure()->rsdt_address;

    // having a RSDT is equivalent to having ACPI supported
    if (acpi_check_sdt_header(&rsdt->header, "RSDT") != 0)
    {
	serial_log(ERROR, "No ACPI was found on this computer!\n");
	kernel_log(ERROR, "No ACPI was found on this computer!\n");


	serial_log(ERROR, "Kernel halted!\n");
	kernel_log(ERROR, "Kernel halted!\n");

	for (;;)
	    asm ("hlt");
    }

    // TODO: maybe this shouldn't be part of acpi_init
    // TODO: continue here
    if (acpi_find_sdt_table("APIC") != NULL)
	debug("we found MADT table!!!!!\n");

    // find MADT using acpi_find_table("APIC") (if not found panic, if return is null)
    // initialize MADT -> after that return it's return value, which is a MADT-struct
    
    serial_log(INFO, "ACPI initialized\n");
    kernel_log(INFO, "ACPI initialized\n");
}

int acpi_check_sdt_header(sdt_header_t *sdt_header, const char *signature)
{
    if (memcmp(sdt_header->signature, signature, 4) == 0 &&
	    acpi_verify_sdt_header_checksum(sdt_header) == 0)
	return 0;

    return 1;
}

int acpi_verify_sdt_header_checksum(sdt_header_t *sdt_header)
{
    uint8_t checksum = 0;
    uint8_t *ptr = (uint8_t *)sdt_header;
    uint8_t current_byte;

    serial_log(INFO, "Verifying %s checksum:\n", sdt_header->signature);
    kernel_log(INFO, "Verifying %s checksum:\n", sdt_header->signature);

    serial_set_color(TERM_PURPLE);

    debug("First %d bytes are being checked: ", sdt_header->length);
    printk(GFX_PURPLE, "First %d bytes are being checked: ", sdt_header->length);

    for (uint8_t i = 0; i < sdt_header->length; i++)
    {
	current_byte = ptr[i];
	debug("%x ", current_byte);
	printk(GFX_PURPLE, "%x ", current_byte);
	
	checksum += current_byte;
    }

    debug("\n");
    printk(GFX_PURPLE, "\n");

    serial_set_color(TERM_COLOR_RESET);

    checksum = checksum & 0xFF;

    if (checksum == 0)
    {
	serial_log(INFO, "%s checksum is verified.\n", sdt_header->signature);
	kernel_log(INFO, "%s checksum is verified.\n", sdt_header->signature);

	return 0;
    }
    else
    {
	serial_log(ERROR, "%s checksum isn't 0! Checksum: 0x%x\n", sdt_header->signature, checksum);
	kernel_log(ERROR, "%s checksum isn't 0! Checksum: 0x%x\n", sdt_header->signature, checksum);

	return 1;
    }
}

// traverse RSDT to find table according to identifier
sdt_header_t *acpi_find_sdt_table(const char *signature)
{
    size_t entry_count = (rsdt->header.length - sizeof(rsdt->header)) / (has_xsdt() ? 8 : 4);
    sdt_header_t *current_entry;

    for (size_t i = 0; i < entry_count; i++)
    {
	current_entry = (sdt_header_t *)(uintptr_t)rsdt->entries[i];	

	if (acpi_check_sdt_header(current_entry, signature) == 0)
	    return current_entry;
    }

    serial_log(ERROR, "Could not find SDT with signature '%s'!\n", signature);
    kernel_log(ERROR, "Could not find SDT with signature '%s'!\n", signature);

    return NULL;
}
