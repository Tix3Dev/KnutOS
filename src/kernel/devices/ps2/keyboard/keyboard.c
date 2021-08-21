/*
	This file is part of an x86_64 hobbyist operating system called KnutOS
	Everything is openly developed on GitHub: https://github.com/Tix3Dev/KnutOS/

	Copyright (C) 2021  Yves Vollmeier <https://github.com/Tix3Dev>
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

// The keyboard IRQ handler function (+ scancode to keycode) is partially copied from this tutorial:
// https://lowlevel.eu/wiki/Keyboard_Controller

#include <stdint.h>

#include <boot/stivale2.h>
#include <devices/pic/pic.h>
#include <devices/ps2/keyboard/keyboard.h>
#include <libk/io/io.h>
#include <libk/debug/debug.h>
#include <libk/stdio/stdio.h>

static uint8_t is_keyboard_active = 0;	// boolean whether keyboard IRQ should get processed

static uint8_t standard_keycodes[128] =
{
	0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
	10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
	20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
	30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
	40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
	50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
	60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
	70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
	80, 81, 82, 84, 00, 00, 86, 87, 88, 00,
	00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
	00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
	00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
	00, 00, 00, 00, 00, 00, 00, 00
};

static uint8_t e0_keycodes[128] =
{
	00,  00,  00,  00,  00,  00,  00,  00,  00,  00,
	00,  00,  00,  00,  00,  00,  00,  00,  00,  00,
	00,  00,  00,  00,  00,  00,  00,  00,  96,  97,
	00,  00,  00,  00,  00,  00,  00,  00,  00,  00,
	00,  00,  00,  00,  00,  00,  00,  00,  00,  00,
	00,  00,  00,  99,  00,  00,  00,  00,  00,  00,
	00,  00,  00,  00,  00,  00,  00,  00,  00,  00,
	00, 102, 103, 104,  00, 105,  00, 106,  00, 107,
	108, 109, 110, 111,  00,  00,  00,  00,  00,  00,
	00,  00,  00,  00,  00,  00,  00,  00,  00,  00,
	00,  00,  00,  00,  00,  00,  00,  00,  00,  00,
	00,  00,  00,  00,  00,  00,  00,  00,  00,  00,
	00,  00,  00,  00,  00,  00,  00,  00
};

// initialize the keyboard
void keyboard_init(void)
{
	// clear the set mask for the keyboard IRQ
	pic_clear_mask(1);

	// clear the data register of the keyboard controller
	while (io_inb(KB_CONTROLLER_COMMAND) & 0x1)
		io_inb(KB_CONTROLLER_DATA);

	// enable scanning (so that the keyboard will send scan codes)
	keyboard_send_command(0xF4);
}

// send a command byte to the keyboard controller
void keyboard_send_command(uint8_t command_byte)
{
	// wait until keyboard is ready
	while (io_inb(KB_CONTROLLER_COMMAND) & 0x2)
		io_outb(KB_CONTROLLER_DATA, command_byte);
}

// gets called whenever a keyboard IRQ occurrs
void keyboard_irq_handler(void)
{
	// this positioning is important
	// because one has to read from the keyboard controller
	// before returning in case of not continuing
	uint8_t scancode = io_inb(KB_CONTROLLER_DATA);

	// check if keyboard IRQ should get processed
	if (!is_keyboard_active)
		return;

	uint8_t		keycode		= 0;
	uint32_t	break_code	= 0;

	static uint32_t	e0_code			= 0;
	static uint32_t e1_code			= 0;
	static uint16_t	first_e1_part	= 0;

	// check for breakcode
	if ((scancode & 0x80)				&&
			(e1_code || (scancode != 0xE1))	&&
			(e0_code || (scancode != 0xE0)))
	{
		break_code	= 1;
		scancode	&= ~0x80;
	}

	// check if e0 was read
	if (e0_code)
	{
		// check for fake shifts
		if ((scancode == 0x2A) || (scancode == 0x36))
		{
			e0_code = 0;
			return;
		}

		keycode = scancode_to_keycode(1, scancode);
		e0_code = 0;
	}
	// check if e1 was read
	else if (e1_code == 1)
	{
		first_e1_part = scancode;
		e1_code++;
	}
	// check if first data byte was read
	else if (e1_code == 2)
	{
		first_e1_part	|= ((uint16_t) scancode << 8);
		keycode			= scancode_to_keycode(2, first_e1_part);
		e1_code			= 0;
	}
	// check for beginning of a e0 code
	else if (scancode == 0xE0)
		e0_code = 1;
	// check for beginning of a e1 code
	else if (scancode == 0xE1)
		e1_code = 1;
	// else -> normal scancode
	else
		keycode = scancode_to_keycode(0, scancode);

	printk(GFX_BLUE, "%c", keycode);
}

uint8_t scancode_to_keycode(uint8_t scancode_set, uint16_t scancode)
{
	uint8_t keycode = 0;

	// standard scancode set
	if (scancode_set == 0)
		keycode = standard_keycodes[scancode];
	// extended 0 scancode set
	else if (scancode_set == 1)
		keycode = e0_keycodes[scancode];
	// extended 1 scancode set - pause
	else if (scancode_set == 2 && scancode == 0x451D)
		keycode = 119;
	// extended 1 scancode set - rest
	else if (scancode_set == 2 && scancode != 0x451D)
		keycode = 0;

	if (keycode == 0)
	{
		serial_set_color(TERM_RED);
		debug("\n──────────────────────────────\n");
		debug("⚠ UNKNOWN SCANCODE RECEIVED! ⚠\n\n");
		debug("⤷ Scancode: 0x%x | Set: %d\n", scancode, scancode_set);
		serial_set_color(TERM_COLOR_RESET);
	}

	return keycode;
}

void activate_keyboard_processing(void)
{
	is_keyboard_active = 1;
}

void disable_keyboard_processing(void)
{
	is_keyboard_active = 0;
}
