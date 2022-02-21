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

// fill a block of memory with a certain value
void *memset(void *pointer, uint32_t value, size_t size)
{
    uint8_t *buffer_pointer = (uint8_t *)pointer;

    for (size_t i = 0; i < size; i++)
        *buffer_pointer++ = value;

    return pointer;
}

// compares the first n bytes of memory area string1 and memory are string2
// return value is
// < 0 when string1 is less than string2
// > 0 when string1 is greater than string2
// = 0 when string1 is equal to string2
// return value is based of fist non-matching character (e.g. abc aBc -> b/B)
int memcmp(const void *string1, const void *string2, size_t n)
{
    const unsigned char *str1 = string1;
    const unsigned char *str2 = string2;

    for (size_t i = 0; i < n; i++, str1++, str2++)
    {
        if (*str1 != *str2)
            return (*str1 - *str2);
    }

    return 0;
}
