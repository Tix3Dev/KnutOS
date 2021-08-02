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

# C_FILES		= $(shell find src/ -type f -name '*.c')
# 
# 
# format:
#	astyle --mode=c -nA1TfpxgHxbxjxpS $(C_FILES)



ISO_IMAGE = disk.iso

.PHONY: all run clean distclean

all: $(ISO_IMAGE)

run: $(ISO_IMAGE)
	qemu-system-x86_64 -M q35 -m 2G -cdrom $(ISO_IMAGE)

limine:
#	git clone https://github.com/limine-bootloader/limine.git --branch=v2.0-branch-binary --depth=1
	make -C third_party/limine

src/kernel/kernel.elf:
	$(MAKE) -C kernel

$(ISO_IMAGE): limine src/kernel/kernel.elf
	rm -rf iso_root
	mkdir -p iso_root
	cp src/kernel/kernel.elf \
		limine.cfg third_party/limine/limine.sys third_party/limine/limine-cd.bin third_party/limine/limine-eltorito-efi.bin iso_root/
	xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-eltorito-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		iso_root -o $(ISO_IMAGE)
	third_party/limine/limine-install $(ISO_IMAGE)
	rm -rf iso_root

clean:
	rm -f $(ISO_IMAGE)
	$(MAKE) -C kernel clean

distclean: clean
	rm -rf limine src/kernel/stivale2.h
