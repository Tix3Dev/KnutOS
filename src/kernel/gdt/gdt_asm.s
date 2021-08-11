;	This file is part of an x86_64 hobbyist operating system called KnutOS
;	Everything is openly developed on GitHub: https://github.com/Tix3Dev/KnutOS/
;	
;	Copyright (C) 2021  Yves Vollmeier <https://github.com/Tix3Dev>
;	This program is free software: you can redistribute it and/or modify
;	it under the terms of the GNU General Public License as published by
;	the Free Software Foundation, either version 3 of the License, or
;	(at your option) any later version.
;	This program is distributed in the hope that it will be useful,
;	but WITHOUT ANY WARRANTY; without even the implied warranty of
;	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;	GNU General Public License for more details.
;	You should have received a copy of the GNU General Public License
;	along with this program.  If not, see <https://www.gnu.org/licenses/>.

global _load_gdt_and_tss_asm

_load_gdt_and_tss_asm:
	lgdt	[rdi]			; load GDT -> rdi is the first argument passed
							; _load_gdt_and_tss_asm(struct GDT_Pointer *ptr)
	
	mov		ax, 0x40		; 0x40 is the TSS segment 
	ltr		ax				; load TSS

	mov		ax,	0x10		; 0x10 is the kernel data segment
	mov		ds, ax			; load kernel data segment (into data segment registers)
	mov		es, ax
	mov		fs, ax
	mov		gs, ax
	mov		ss, ax
	
	pop		rdi				; pop return address
	mov		rax, 0x08		; 0x08 is the kernel code segment
	push	rax				; push the kernel code segment
	push	rdi				; push return address
	retfq					; far return
