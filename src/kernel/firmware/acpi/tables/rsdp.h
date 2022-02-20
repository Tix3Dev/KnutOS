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

#ifndef RSDP_H
#define RSDP_H

typedef struct __attribute__((__packed__)) 
{
    // ACPI version 1.0
    char	signature[8];
    uint8_t 	checksum;
    char    	oemid[6];
    uint8_t 	revision;
    uint32_t	rsdt_address;

    // ACPI version 2.0+
    uint32_t	length;
    uint64_t	xsdt_address;
    uint8_t	extended_checksum;
    uint8_t	reserved[3];

} rsdp_structure_t;

void rsdp_init(uint64_t rsdp_address);
void rsdp_verify_checksum(uint64_t rsdp_address);
rsdp_structure_t *get_rsdp_structure(void);
bool has_xsdt(void);

#endif
