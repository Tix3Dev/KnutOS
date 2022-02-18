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

#include <stdbool.h>

#include <boot/stivale2.h>
#include <boot/stivale2_boot.h>
#include <firmware/acpi/tables/rsdp.h>
#include <libk/debug/debug.h>
#include <libk/log/log.h>
#include <libk/stdio/stdio.h>

static rsdp_structure_t rsdp;
static bool has_xsdt_var = false;

void rsdp_init(uint64_t rsdp_address)
{
    rsdp_verify_checksum(rsdp_address);
    
    rsdp = *(rsdp_structure_t *)rsdp_address;

    serial_set_color(TERM_PURPLE);

    debug("ACPI Revision number: %d\n", rsdp.revision);
    if (rsdp.revision >= 2)
	has_xsdt_var = true;

    serial_set_color(TERM_COLOR_RESET);
}

// sum up the first 20 bytes of RSDP (starting with rsdp_address)
// and check if this number is equal to zero
void rsdp_verify_checksum(uint64_t rsdp_address)
{
    uint8_t checksum = 0;
    uint8_t *ptr = (uint8_t *)rsdp_address;
    uint8_t current_byte;

    serial_log(INFO, "Verifying RSDP checksum:\n");
    kernel_log(INFO, "Verifying RSDP checksum:\n");

    serial_set_color(TERM_PURPLE);

    debug("20 first bytes are being checked: ");
    printk(GFX_PURPLE, "20 first bytes are being checked: ");

    for (uint8_t i = 0; i < 20; i++)
    {
	current_byte = ptr[i];
	debug("%x ", current_byte);
	printk(GFX_PURPLE, "%x ", current_byte);
	
	checksum += current_byte;
    }

    debug("\n");
    printk(GFX_PURPLE, "\n");

    serial_set_color(TERM_COLOR_RESET);

    if ((checksum & 0xFF) == 0x00)
    {
	serial_log(INFO, "RSDP checksum is verified.\n");
	kernel_log(INFO, "RSDP checksum is verified.\n");
    }
    else
    {
	serial_log(ERROR, "RSDP checksum isn't 0! Checksum: 0x%x\n", checksum & 0xFF);
	kernel_log(ERROR, "RSDP checksum isn't 0! Checksum: 0x%x\n", checksum & 0xFF);

	serial_log(ERROR, "Kernel halted!\n");
	kernel_log(ERROR, "Kernel halted!\n");

	for (;;)
	    asm ("hlt");
    }
}

rsdp_structure_t get_rsdp_structure(void)
{
    return rsdp;
}

bool has_xsdt(void)
{
    return has_xsdt_var;
}
