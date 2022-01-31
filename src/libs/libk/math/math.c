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

// calculate the power of a number using a given base and exponent
// note that the result will always we 0, if the exponent is negative
// algorithm used: https://en.wikipedia.org/wiki/Exponentiation_by_squaring
uint32_t pow(int32_t base, int32_t exponent)
{
    if (exponent == 0)
        return 1;

    if (exponent < 0)
    {
        base = 1 / base;
        exponent = -exponent;
    }

    int32_t temp = 1;

    while (exponent > 1)
    {
        if (exponent % 2 == 0)
        {
            base = base * base;
            exponent = exponent / 2;
        }
        else
        {
            temp = base * temp;
            base = base * base;
            exponent = (exponent - 1) / 2;
        }
    }

    return base * temp;
}
