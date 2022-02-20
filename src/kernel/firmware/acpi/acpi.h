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

#include <firmware/acpi/tables/sdth.h>

#ifndef ACPI_H
#define ACPI_H


void acpi_init(struct stivale2_struct *stivale2_struct);
int acpi_check_sdt_header(sdt_header_t *sdt_header, const char *signature);
int acpi_verify_sdt_header_checksum(sdt_header_t *sdt_header);
sdt_header_t *acpi_find_sdt_table(const char *signature);

#endif
