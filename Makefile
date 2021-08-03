#    This file is part of an x86_64 hobbyist operating system called KnutOS
#    Everything is openly developed on GitHub: https://github.com/Tix3Dev/KnutOS/
#
#    Copyright (C) 2021  Yves Vollmeier <https://github.com/Tix3Dev>
#     This program is free software: you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published by
#     the Free Software Foundation, either version 3 of the License, or
#     (at your option) any later version.
#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.

TARGET		:= src/kernel/kernel.elf
ISO_IMAGE	:= disk.iso

CC = cc
LD = ld

CC_FLAGS = -Wall -Wextra -O2 -pipe
LD_FLAGS =

INTERNAL_LD_FLAGS :=		\
	-Tsrc/kernel/linker.ld	\
	-nostdlib				\
	-zmax-page-size=0x1000	\
	-static					\
	-pie					\
	--no-dynamic-linker		\
	-ztext

INTERNAL_CC_FLAGS :=		\
	-I.						\
	-std=gnu11				\
	-ffreestanding			\
	-fno-stack-protector	\
	-fno-pic -fpie			\
	-mno-80387				\
	-mno-mmx				\
	-mno-3dnow				\
	-mno-sse				\
	-mno-sse2				\
	-mno-red-zone

C_FILES		:= $(shell find src/ -type f -name '*.c')

OBJ			:= $(C_FILES:.c=.o)

.PHONY: all clean format run

all: $(TARGET)

run: $(ISO_IMAGE)
	qemu-system-x86_64 -M q35 -m 2G -cdrom $(ISO_IMAGE)

limine:
	make -C third_party/limine

$(TARGET): $(OBJ)
	$(LD) $(OBJ) $(LD_FLAGS) $(INTERNAL_LD_FLAGS) -o $@

$(ISO_IMAGE): limine $(TARGET)
	rm -rf iso_root
	mkdir -p iso_root
	cp $(TARGET) 												\
		limine.cfg third_party/limine/limine.sys				\
		third_party/limine/limine-cd.bin 						\
		third_party/limine/limine-eltorito-efi.bin iso_root/
	xorriso -as mkisofs -b limine-cd.bin							\
		-no-emul-boot -boot-load-size 4 -boot-info-table			\
		--efi-boot limine-eltorito-efi.bin							\
		-efi-boot-part --efi-boot-image --protective-msdos-label	\
		iso_root -o $(ISO_IMAGE)
	third_party/limine/limine-install $(ISO_IMAGE)
	rm -rf iso_root

%.o: %.c
	@printf "[CC]  $<\n";
	$(CC) $(CC_FLAGS) $(INTERNAL_CC_FLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET) $(OBJ) $(ISO_IMAGE)

format:
	astyle --mode=c -nA1TfpxgHxbxjxpS $(C_FILES)
