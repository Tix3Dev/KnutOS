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
#include <stddef.h>

#include <boot/stivale2.h>
#include <libk/alloc/bitmap.h>

// set exactly one bit to 1 in the bitmap
void bitmap_set_bit(BITMAP_t *bitmap, int bit)
{
    bitmap->map[bit / 8] |= (1 << (bit % 8));
}

// set exactly one bit to 0 in the bitmap
void bitmap_unset_bit(BITMAP_t *bitmap, int bit)
{
    bitmap->map[bit / 8] &= ~(1 << (bit % 8));
}

// check the value (either 0 or 1) for exactly one bit in the bitmap
uint8_t bitmap_check_bit(BITMAP_t *bitmap, int bit)
{
    return bitmap->map[bit / 8] & (1 << (bit % 8));
}
