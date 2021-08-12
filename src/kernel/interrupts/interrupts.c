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

#include <boot/stivale2.h>
#include <interrupts/interrupts.h>
#include <libk/debug/debug.h>
#include <libk/stdio/stdio.h>

static const char *exceptions[] =
{
	"#DE: Divide Error",
	"#DB: Debug Exception",
	" — : NMI Interrupt",
	"#BP: Breakpoint",
	"#OF: Overflow",
	"#BR: BOUND Range Exceeded",
	"#UD: Invalid Opcode (Undefined Opcode)",
	"#NM: Device Not Available (No Math Coprocessor)",
	"#DF: Double Fault",
	"— : Coprocessor Segment Overrun (reserved)",
	"#TS: Invalid TSS",
	"#NP: Segment Not Present",
	"#SS: Stack-Segment Fault",
	"#GP: General Protection",
	"#PF: Page Fault",
	"— : (Intel reserved. Do not use.)",
	"#MF: x87 FPU Floating-Point Error (Math Fault)",
	"#AC: Alignment Check",
	"#MC: Machine Check",
	"#XM: SIMD Floating-Point Exception",
	"#VE: Virtualization Exception",
	"#CP: Control Protection Exception",
	"— : Intel reserved. Do not use.",
	"— : Intel reserved. Do not use.",
	"— : Intel reserved. Do not use.",
	"— : Intel reserved. Do not use.",
	"— : Intel reserved. Do not use.",
	"— : Intel reserved. Do not use.",
	"— : Intel reserved. Do not use.",
	"— : Intel reserved. Do not use.",
	"— : Intel reserved. Do not use.",
	"— : Intel reserved. Do not use."
};

void isr_handler(struct CPU_State *cpu)
{
	if (cpu->interrupt_number < 32)
	{
		serial_set_color(TERM_RED);
		debug("\n────────────────────────\n");
		debug("⚠ EXCEPTION OCCURRED! ⚠\n\n");
		debug("⤷ No. %d: %s\n", cpu->interrupt_number, exceptions[cpu->interrupt_number]);
		serial_set_color(TERM_COLOR_RESET);

		framebuffer_reset_screen();
		printk(GFX_RED, "\n────────────────────────\n");
		printk(GFX_RED, "⚠ EXCEPTION OCCURRED! ⚠\n\n");
		printk(GFX_RED, "⤷ No. %d: %s\n", cpu->interrupt_number, exceptions[cpu->interrupt_number]);

		while (1)
			asm volatile("cli; hlt");
	}
	else
	{
		// handle IRQ
	}

}
