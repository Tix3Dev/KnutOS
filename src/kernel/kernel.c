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
#include <memory/pmm.h>
#include <shell/shell_screen.h>
#include <logo.h>
#include <libk/debug/debug.h>
#include <libk/log/log.h>
#include <libk/stdio/stdio.h>

void kmain(struct stivale2_struct *stivale2_struct)
{
	serial_init();

	serial_set_color(TERM_BLUE);
	debug("Welcome to:\n");
	debug("%s", small_logo_text);
	serial_set_color(TERM_COLOR_RESET);

	log(INFO, __FILE__, "Kernel started\n");

	gdt_init();
	idt_init();
	pmm_init(stivale2_struct);


	// physical memory management test

	serial_set_color(TERM_PURPLE);

	uint32_t *pointer1 = (uint32_t *)pmm_alloc(1);
	debug("pointer1 allocated at 0x%x\n", pointer1);

	uint32_t *pointer2 = (uint32_t *)pmm_alloc(1);
	debug("pointer2 allocated at 0x%x\n", pointer2);

	pmm_free(pointer1, 1);
	debug("pointer1 freed\n");

	pmm_free(pointer2, 1);
	debug("pointer2 freed\n\n");


	uint64_t *pointer3 = (uint64_t *)pmm_alloc(1);
	debug("pointer3 allocated at 0x%x\n", pointer3);

	uint64_t *pointer4 = (uint64_t *)pmm_alloc(1);
	debug("pointer4 allocated at 0x%x\n", pointer4);

	uint64_t *pointer5 = (uint64_t *)pmm_alloc(1);
	debug("pointer5 allocated at 0x%x\n", pointer5);

	pmm_free(pointer3, 1);
	debug("pointer3 freed\n");

	pmm_free(pointer4, 1);
	debug("pointer4 freed\n");

	pmm_free(pointer5, 1);
	debug("pointer5 freed\n");


	uint64_t *pointer6 = (uint64_t *)pmm_alloc(1);
	debug("pointer6 allocated at 0x%x\n", pointer6);

	pmm_free(pointer6, 1);
	debug("pointer6 freed\n");

	serial_set_color(TERM_COLOR_RESET);


	framebuffer_init(stivale2_struct, GFX_BLACK);
	keyboard_init();								// NOTE: is_keyboard_active is still false so no processing

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
