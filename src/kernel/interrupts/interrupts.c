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
#include <devices/pic/pic.h>
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
	// handle exceptions
	if (cpu->isr_number <= 31)
	{
		serial_set_color(TERM_RED);
		debug("\n────────────────────────\n");
		debug("⚠ EXCEPTION OCCURRED! ⚠\n\n");
		debug("⤷ ISR-No. %d: %s\n\n\n", cpu->isr_number, exceptions[cpu->isr_number]);
		debug("ℹ Register dump:\n\n");
		debug("⤷ rax: 0x%.8x, rbx:    0x%.8x, rcx: 0x%.8x, rdx: 0x%.8x\n"
		      "⤷ rsi: 0x%.8x, rdi:    0x%.8x, rbp: 0x%.8x, r8 : 0x%.8x\n"
			  "⤷ r9 : 0x%.8x, r10:    0x%.8x, r11: 0x%.8x, r12: 0x%.8x\n"
			  "⤷ r13: 0x%.8x, r14:    0x%.8x, r15: 0x%.8x, ss : 0x%.8x\n"
			  "⤷ rsp: 0x%.8x, rflags: 0x%.8x, cs : 0x%.8x, rip: 0x%.8x\n",
		      cpu->rax, cpu->rbx,    cpu->rcx, cpu->rdx,
		      cpu->rsi, cpu->rdi,    cpu->rbp, cpu->r8,
		      cpu->r9,  cpu->r10,    cpu->r11, cpu->r12,
		      cpu->r13, cpu->r14,    cpu->r15, cpu->ss,
		      cpu->rsp, cpu->rflags, cpu->cs,  cpu->rip);

		serial_set_color(TERM_COLOR_RESET);

		framebuffer_reset_screen();
		printk(GFX_RED, 	"\n────────────────────────\n");
		printk(GFX_RED, 	"⚠ EXCEPTION OCCURRED! ⚠\n\n");
		printk(GFX_RED, 	"⤷ ISR-No. %d: %s\n\n\n", cpu->isr_number, exceptions[cpu->isr_number]);
		printk(GFX_CYAN,	"ℹ Register dump:\n\n");
		printk(GFX_CYAN,	"⤷ rax: 0x%.8x, rbx:    0x%.8x, rcx: 0x%.8x, rdx: 0x%.8x\n"
						    "⤷ rsi: 0x%.8x, rdi:    0x%.8x, rbp: 0x%.8x, r8 : 0x%.8x\n"
						    "⤷ r9 : 0x%.8x, r10:    0x%.8x, r11: 0x%.8x, r12: 0x%.8x\n"
						    "⤷ r13: 0x%.8x, r14:    0x%.8x, r15: 0x%.8x, ss : 0x%.8x\n"
						    "⤷ rsp: 0x%.8x, rflags: 0x%.8x, cs : 0x%.8x, rip: 0x%.8x\n",
						    cpu->rax, cpu->rbx,    cpu->rcx, cpu->rdx,
						    cpu->rsi, cpu->rdi,    cpu->rbp, cpu->r8,
						    cpu->r9,  cpu->r10,    cpu->r11, cpu->r12,
						    cpu->r13, cpu->r14,    cpu->r15, cpu->ss,
						    cpu->rsp, cpu->rflags, cpu->cs,  cpu->rip);

		// clear interrupts and halt forever
		while (1)
			asm volatile("cli; hlt");
	}
	// handle IRQ's / hardware interrupts
	else if (cpu->isr_number >= 32 && cpu->isr_number <= 47)
	{
		if (cpu->isr_number == 32)
		{
			debug("timer IRQ occurred");
		}

		pic_signal_EOI(cpu->isr_number);
	}
}
