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
#include <firmware/acpi/acpi.h>
#include <libk/debug/debug.h>
#include <libk/log/log.h>
#include <libk/stdio/stdio.h>
#include <libk/string/string.h>

void acpi_init(struct stivale2_struct *stivale2_struct)
{
    struct stivale2_struct_tag_rsdp *rsdp_tag = stivale2_get_tag(stivale2_struct,
	    STIVALE2_STRUCT_TAG_RSDP_ID);

    rsdp_init(rsdp_tag->rsdp);

    if (acpi_check_header(get_rsdp_structure().rsdt_address, "RSDT") == 0)
	debug("hello\n");

    // init RSDP
    //	-> verify checksum (all entries sum up to zero)
    //	-> set global RSDP struct which can be used later

    // find MADT using acpi_find_table("APIC") (if not found panic, if return is null)
    // initialize MADT -> after that return it's return value, which is a MADT-struct
    
    serial_log(INFO, "ACPI initialized\n");
    kernel_log(INFO, "ACPI initialized\n");
}

int acpi_check_header(uint64_t address, const char *signature)
{
    if (memcmp((void *)address, signature, 4) == 0 &&
	    acpi_verify_checksum(address, signature) == 0)
	return 0;

    return 1;
}

int acpi_verify_checksum(uint64_t address, const char *signature)
{
    uint8_t checksum = 0;
    uint8_t *ptr = (uint8_t *)address;
    uint8_t current_byte;

    serial_log(INFO, "Verifying %s checksum:\n", signature);
    kernel_log(INFO, "Verifying %s checksum:\n", signature);

    serial_set_color(TERM_PURPLE);

    debug("20 first bytes are being checked: ");
    printk(GFX_PURPLE, "20 first bytes are being checked: ");

    for (uint8_t i = 0; i < *(ptr + 1); i++)
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
	serial_log(INFO, "%s checksum is verified.\n", signature);
	kernel_log(INFO, "%s checksum is verified.\n", signature);

	return 0;
    }
    else
    {
	serial_log(ERROR, "%s checksum isn't 0! Checksum: 0x%x\n", signature, checksum);
	kernel_log(ERROR, "%s checksum isn't 0! Checksum: 0x%x\n", signature, checksum);

	return 1;
    }
}

void acpi_find_table(const char *identifier)
{
    // NOTE: make use of has_xsdt for length!!!

    // convert RSDP to RSDT
    
    // traverse RSDT struct which should hold different tables, that are SDT or smth
    // return table if sdt->signature == identifier and if verified SDT checksum (all
    // entries sum up to zero)
}
