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

#ifndef SDTH_H
#define SDTH_H

typedef struct __attribute__((__packed__))
{
    char	signature[4];
    uint32_t	length;
    uint8_t	revision;
    uint8_t	checksum;
    char	oem_string[6];
    char	oem_table_id[8];
    uint32_t	oem_revision;
    char	creator_id[4];
    uint32_t	creator_revision;
} sdt_header_t;

#endif
