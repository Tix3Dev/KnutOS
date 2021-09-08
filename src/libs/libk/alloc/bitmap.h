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

#ifndef BITMAP_H
#define BITMAP_H

// void bitmap_set_bit(uint32_t *bitmap, int bit);
void bitmap_set_bit(struct stivale2_struct_tag_memmap *bitmap, int bit);
// void bitmap_unset_bit(uint32_t *bitmap, int bit);
void bitmap_unset_bit(struct stivale2_struct_tag_memmap *bitmap, int bit);
// uint8_t bitmap_check_bit(uint32_t *bitmap, int bit);
uint8_t bitmap_check_bit(struct stivale2_struct_tag_memmap *bitmap, int bit);

#endif
