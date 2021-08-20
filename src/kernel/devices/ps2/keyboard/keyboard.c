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

#include <stdint.h>

#include <devices/pic/pic.h>
#include <devices/ps2/keyboard/keyboard.h>
#include <libk/io/io.h>


#include <libk/debug/debug.h>


static uint8_t is_keyboard_active = 0;	// boolean whether keyboard IRQ should get processed

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
	uint8_t scancode = io_inb(KB_CONTROLLER_DATA);

	// check if keyboard IRQ should get processed
	if (!is_keyboard_active)
		return;

	debug("keyboard IRQ occurred at valid time");
}

void activate_keyboard_processing(void)
{
	is_keyboard_active = 1;
}

void disable_keyboard_processing(void)
{
	is_keyboard_active = 0;
}
