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

#include <stddef.h>
#include <stdint.h>

#include <boot/boot_screen.h>
#include <boot/stivale2.h>
#include <boot/stivale2_boot.h>
#include <devices/apic/apic.h>
#include <devices/cpu/cpu.h>
#include <devices/ps2/keyboard/keyboard.h>
#include <firmware/acpi/acpi.h>
#include <gdt/gdt.h>
#include <interrupts/idt.h>
#include <memory/pmm.h>
#include <memory/slab.h>
#include <memory/vmm.h>
#include <shell/shell_screen.h>
#include <logo.h>
#include <libk/debug/debug.h>
#include <libk/log/log.h>
#include <libk/stdio/stdio.h>

void kmain(struct stivale2_struct *stivale2_struct)
{
    framebuffer_init(stivale2_struct, GFX_BLACK);
    serial_init();

    serial_log(INFO, "Framebuffer and serial initialized\n");
    kernel_log(INFO, "Framebuffer and serial initialized\n");

    serial_set_color(TERM_BLUE);
    debug("Welcome to:\n");
    debug("%s", small_logo_text);
    serial_set_color(TERM_COLOR_RESET);

    serial_log(INFO, "Kernel started\n");
    kernel_log(INFO, "Kernel started\n");

    pmm_init(stivale2_struct);
    vmm_init();
    gdt_init();
    idt_init();

    slab_init();

    char *vendor_string = cpu_get_vendor_string();
    serial_log(INFO, "CPU Vendor ID String: %s\n", vendor_string);
    kernel_log(INFO, "CPU Vendor ID String: %s\n", vendor_string);

    acpi_init(stivale2_struct);

    apic_init();

    // keyboard_init(); // NOTE: is_keyboard_active is still false so no processing

    // TODO: proper timer
    // for (long i = 0; i < 5500000000; i++)	// ~10 seconds
    // 	asm ("nop");

    // shell_screen_init();

    for (;;)
        asm ("hlt");
}
