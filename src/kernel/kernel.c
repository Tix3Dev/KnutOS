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

#include <stddef.h>
#include <stdint.h>

#include <boot/stivale2.h>
#include <boot/stivale2_boot.h>
#include <devices/ps2/keyboard/keyboard.h>
#include <gdt/gdt.h>
#include <interrupts/idt.h>
#include <shell/shell_screen.h>
#include <logo.h>
#include <libk/debug/debug.h>
#include <libk/log/log.h>
#include <libk/stdio/stdio.h>

void kmain(struct stivale2_struct *stivale2_struct)
{
	serial_init();

	log(__FILE__, "Kernel started\n");

	gdt_init();
	idt_init();
	framebuffer_init(stivale2_struct, GFX_BLACK);
	keyboard_init();								// NOTE: is_keyboard_active is still false so no processing

	serial_set_color(TERM_BLUE);

	debug("Welcome to:\n");
	debug("%s", small_logo_text);
	serial_set_color(TERM_COLOR_RESET);

	printk(GFX_BLUE, "\nWelcome to:\n");
	printk(GFX_BLUE, "%s", big_logo);

	// printk(GFX_BLACK,	"█color█ "); /* same color as background so no need to show it */
	printk(GFX_RED,		"█color█ ");
	printk(GFX_GREEN,	"█color█ ");
	printk(GFX_YELLOW,	"█color█ ");
	printk(GFX_BLUE,	"█color█ ");
	printk(GFX_PURPLE,	"█color█ ");
	printk(GFX_CYAN,	"█color█ ");
	printk(GFX_WHITE,	"█color█\n");

	// TODO: proper timer
	for (long i = 0; i < 5500000000; i++)	// ~10 seconds
	asm ("nop");

	shell_screen_init();

	for (;;)
		asm ("hlt");
}
