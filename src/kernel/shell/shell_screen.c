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
#include <libk/ssfn.h>

// create a "new" screen and print a basic shell prompt
// after that activate keyboard processing and configure it to call
// shell_print_char
void shell_screen_init(void)
{
	framebuffer_reset_screen();
	shell_prompt();

	// set barrier at last cursor position (ssfn_dst x/y)
	activate_keyboard_processing(*shell_print_char, ssfn_dst.x, ssfn_dst.y, ssfn_dst.x+100, ssfn_dst.y+100);
}

// print basic shell prompt
void shell_prompt(void)
{
	printk(GFX_PURPLE, "┌─ KnutOS\n");
	printk(GFX_PURPLE, "└→ ");
}

// get informations about what was being pressed
// and just print if it is a valid ascii_character
void shell_print_char(KEY_INFO_t key_info)
{
	if (key_info.ascii_character != '\0')
		printk(GFX_BLUE, "%c", key_info.ascii_character);
}
