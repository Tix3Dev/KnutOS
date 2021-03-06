#	This file is part of an x86_64 hobbyist operating system called KnutOS
#	Everything is openly developed on GitHub: https://github.com/Tix3Dev/KnutOS/
#	
#	Copyright (C) 2021-2022  Yves Vollmeier <https://github.com/Tix3Dev>
#	This program is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#	This program is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#	You should have received a copy of the GNU General Public License
#	along with this program.  If not, see <https://www.gnu.org/licenses/>.

TARGET		:= src/kernel/kernel.elf
ISO_IMAGE	:= disk.iso

ARCH = @x86_64-elf

CC	= $(ARCH)-gcc
AS	= @nasm
LD	= $(ARCH)-ld

# for full optimization use -O3 and don't use -ggdb
# currently it's balanced out between optimization and debugging
CC_FLAGS	= -Wall -Wextra -O2 -pipe -ggdb
AS_FLAGS	= -felf64
LD_FLAGS	=

INTERNAL_LD_FLAGS :=		\
	-Tsrc/kernel/linker.ld	\
	-nostdlib				\
	-static					\
	-zmax-page-size=0x1000	\
	--no-dynamic-linker		\
	-ztext					
	# -pie

INTERNAL_CC_FLAGS :=		\
	-Isrc/kernel/			\
	-Isrc/libs				\
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
AS_FILES	:= $(shell find src/ -type f -name '*.s')

C_OBJ	= $(C_FILES:.c=.o)
AS_OBJ	= $(AS_FILES:.s=.o)
OBJ		= $(C_OBJ) $(AS_OBJ)

.PHONY: all clean format run

all: $(TARGET)

run: $(ISO_IMAGE)
	qemu-system-x86_64 -M q35 -m 2G -serial stdio -cdrom $(ISO_IMAGE)

debug: $(ISO_IMAGE)
	qemu-system-x86_64 -M q35 -m 2G -serial stdio -cdrom $(ISO_IMAGE) -s -S

limine:
	make -C third_party/limine

$(TARGET): $(OBJ)
	$(LD) -r -b binary -o src/kernel/devices/framebuffer/font_unifont.o sfn_fonts/unifont.sfn
	$(LD) src/kernel/devices/framebuffer/font_unifont.o $(OBJ) $(LD_FLAGS) $(INTERNAL_LD_FLAGS) -o $@
	@printf "\n\n \(^ᴗ^)/ Kernel compiled and linked successfully \(^ᴗ^)/ \n\n"


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
	@printf " [CC]\t$<\n";
	$(CC) $(CC_FLAGS) $(INTERNAL_CC_FLAGS) -c $< -o $@

%.o: %.s
	@printf " [AS]\t$<\n";
	$(AS) $(AS_FLAGS) $< -o $@

clean:
	rm -rf $(TARGET) $(OBJ) $(ISO_IMAGE)

format:
	astyle --mode=c -nA1fpxgHxbxjxpS $(C_FILES)
