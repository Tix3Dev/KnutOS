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

/* Tell the linker that we want an x86_64 ELF64 output file */
OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
 
/* We want the symbol _start to be our entry point */
ENTRY(_start)
 
/* Define the program headers we want so the bootloader gives us the right */
/* MMU permissions */
PHDRS
{
    null    PT_NULL    FLAGS(0) ;                   /* Null segment */
    text    PT_LOAD    FLAGS((1 << 0) | (1 << 2)) ; /* Execute + Read */
    rodata  PT_LOAD    FLAGS((1 << 2)) ;            /* Read only */
    data    PT_LOAD    FLAGS((1 << 1) | (1 << 2)) ; /* Write + Read */
    dynamic PT_DYNAMIC FLAGS((1 << 1) | (1 << 2)) ; /* Dynamic segment needed for PIE */
}
 
SECTIONS
{
    /* We wanna be placed in the higher half, 2MiB above 0 in physical memory. */
    /* Since we are going to use PIE, this is just the base load address, but the */
    /* bootloader will be able to relocate us as it sees fit. */
    . = 0xffffffff80200000;
 
    .text : {
        *(.text*)
    } :text
 
    /* Move to the next memory page for .rodata */
    . += 0x1000;
 
    /* We place the .stivale2hdr section containing the header in its own section, */
    /* and we use the KEEP directive on it to make sure it doesn't get discarded. */
    .stivale2hdr : {
        KEEP(*(.stivale2hdr))
    } :rodata
 
    .rodata : {
        *(.rodata*)
    } :rodata
 
    /* Move to the next memory page for .data */
    . += 0x1000;
 
    .data : {
        *(.data*)
    } :data
 
    /* Dynamic section needed for PIE */
    .dynamic : {
        *(.dynamic)
    } :data :dynamic
 
    .bss : {
        *(COMMON)
        *(.bss*)
    } :data
}
