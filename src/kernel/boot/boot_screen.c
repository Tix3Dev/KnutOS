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

#include <boot/boot_screen.h>
#include <boot/stivale2.h>
#include <logo.h>
#include <libk/stdio/stdio.h>

// show splashscreen aka silent bootscreen
void silent_boot_screen(void)
{
    printk(GFX_BLUE,	"\nWelcome to:\n");
    printk(GFX_BLUE,	"%s", big_logo);

    // printk(GFX_BLACK,	"█color█ "); /* same color as background so no need to show it */
    printk(GFX_RED,	"█color█ ");
    printk(GFX_GREEN,	"█color█ ");
    printk(GFX_YELLOW,	"█color█ ");
    printk(GFX_BLUE,	"█color█ ");
    printk(GFX_PURPLE,	"█color█ ");
    printk(GFX_CYAN,	"█color█ ");
    printk(GFX_WHITE,	"█color█\n");
}

// show scrolling text information (like in the serial console)
void verbose_boot_screen(void)
{
    //
}

// switch bootscreen by pressing f1
void switch_boot_screen(void)
{
    //
}
