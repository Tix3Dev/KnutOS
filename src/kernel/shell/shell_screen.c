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
#include <devices/ps2/keyboard/keyboard.h>
#include <shell/shell_screen.h>
#include <libk/stdio/stdio.h>

void shell_screen_init(void)
{
	framebuffer_reset_screen();
	activate_keyboard_processing();

	// loop (new iteration after enter has been pressed)
	shell_prompt();
}

void shell_prompt(void)
{
	printk(GFX_PURPLE, "┌─ KnutOS\n");
	printk(GFX_PURPLE, "└→ ");
}
