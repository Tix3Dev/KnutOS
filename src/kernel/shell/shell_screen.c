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

#include <stdint.h>

#include <boot/stivale2.h>
#include <devices/ps2/keyboard/keyboard.h>
#include <shell/shell_screen.h>
#include <libk/stdio/stdio.h>
#include <libk/ssfn.h>

static int shell_prompt_x_barrier;
static int shell_prompt_y_barrier;

// create a "new" screen and print a basic shell prompt
// after that activate keyboard processing and configure it to call
// shell_print_char
void shell_screen_init(void)
{
    framebuffer_reset_screen();
    shell_prompt();

    activate_keyboard_processing(*shell_print_char);
}

// print basic shell prompt
void shell_prompt(void)
{
    printk(GFX_PURPLE, "\n┌─ KnutOS\n");
    printk(GFX_PURPLE, "└→ ");

    // set x, y barrier to last know cursor position
    shell_prompt_x_barrier = ssfn_dst.x;
    shell_prompt_y_barrier = ssfn_dst.y;
}

// get informations about what was being pressed
// and just print if it is a valid ascii_character
void shell_print_char(KEY_INFO_t key_info)
{
    // if it's nothing to print, return
    if (key_info.ascii_character == '\0')
        return;
    // if return is pressed, make a newline and create a new prompt
    else if (key_info.ascii_character == KEY_RETURN)
    {
        // move barrier if screen scolls
        if (ssfn_dst.y + gfx.glyph_height == gfx.fb_height)
            shell_prompt_y_barrier += gfx.glyph_height;

        printk(GFX_BLUE, "\n");
        shell_prompt();
    }
    // if backspace is pressed check for barrier (prompt)
    else if (key_info.ascii_character == KEY_BACKSPACE)
    {
        if (ssfn_dst.x == shell_prompt_x_barrier && ssfn_dst.y == shell_prompt_y_barrier)
            return;

        printk(GFX_BLUE, "\b");
    }
    else
    {
        // move barrier if screen scrolls
        if (ssfn_dst.y + gfx.glyph_height == gfx.fb_height && ssfn_dst.x + gfx.glyph_width == gfx.fb_width)
            shell_prompt_y_barrier -= gfx.glyph_height;

        printk(GFX_BLUE, "%c", key_info.ascii_character);
    }
}
