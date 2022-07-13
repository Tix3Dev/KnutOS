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

#ifndef HPET_H
#define HPET_H

typedef struct acpi_header_t {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oem[6];
    char oem_table[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} __attribute__((packed)) acpi_header_t;

typedef struct acpi_gas_t {
    uint8_t address_space;
    uint8_t bit_width;
    uint8_t bit_offset;
    uint8_t access_size;
    uint64_t base;
} __attribute__((packed)) acpi_gas_t;

struct hpet_table {
	acpi_header_t header;
	uint8_t hardware_rev_id;
	uint8_t comparator_count : 5;
	uint8_t counter_size : 1;
	uint8_t reserved : 1;
	uint8_t legacy_replacement : 1;
	uint16_t pci_vendor_id;
	acpi_gas_t address;
	uint8_t hpet_number;
	uint16_t minimum_tick;
	uint8_t page_protection;
} __attribute__((packed));

struct hpet {
	uint64_t general_capabilities;
	uint64_t reserved;
	uint64_t general_configuration;
	uint64_t reserved2;
	uint64_t general_int_status;
	uint64_t reserved3;
	uint64_t reserved4[24];
	uint64_t main_counter_value;
	uint64_t reserved5;
};

void hpet_init(void);
uint64_t hpet_counter_value(void);
void hpet_usleep(uint64_t us);

bool timer_installed(void);
void timer_sleep(uint64_t ms);
uint64_t timer_count(void);
void timer_stop_sched(void);

#endif
