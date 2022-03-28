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

#include <boot/stivale2.h>
#include <boot/stivale2_boot.h>
#include <devices/pic/pic.h>
#include <devices/pit/pit.h>
#include <libk/debug/debug.h>
#include <libk/io/io.h>
#include <libk/log/log.h>

static uint32_t pit_ticks = 0;

// set up pit to be used
void pit_init(void)
{
    pic_clear_mask(0);

    uint16_t hz_frequency = 1000;
    pit_set_mode(0, 3, 2, 1193182 / hz_frequency);

    serial_log(INFO, "PIT initialized\n");
    kernel_log(INFO, "PIT initialized\n");
}

// increment pit timer ticks
void pit_irq_handler(void)
{
    pit_ticks++;
}

// put the pit into one of many possible modes
// by setting select channel, access mode, operating mode and freqeuncy
void pit_set_mode(uint8_t select_channel, uint8_t access_mode, uint8_t operating_mode, uint16_t frequency)
{
    // send mode/command byte
    io_outb(0x43, (select_channel << 6) | (access_mode << 4) | (operating_mode << 1));

    // send the frequency divider to the input channel
    io_outb(0x40 + select_channel, (uint8_t)frequency);
    io_outb(0x40 + select_channel, (uint8_t)(frequency >> 8));
}

// sleep for specific amount of ms
// NOTE: the accuracy of this will vary between different environments
void pit_sleep_ms(uint16_t ms)
{
    uint64_t end_tick_time = pit_ticks + ms;

    while (pit_ticks < end_tick_time)
	asm volatile("sti; hlt");
}
