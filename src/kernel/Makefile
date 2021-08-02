KERNEL := kernel.elf

CC = cc
LD = ld

CFLAGS = -Wall -Wextra -O2 -pipe
LDFLAGS =

INTERNALLDFLAGS :=         \
	-Tlinker.ld            \
	-nostdlib              \
	-zmax-page-size=0x1000 \
	-static                \
	-pie                   \
	--no-dynamic-linker    \
	-ztext

INTERNALCFLAGS  :=       \
	-I.                  \
	-std=gnu11           \
	-ffreestanding       \
	-fno-stack-protector \
	-fno-pic -fpie       \
	-mno-80387           \
	-mno-mmx             \
	-mno-3dnow           \
	-mno-sse             \
	-mno-sse2            \
	-mno-red-zone

CFILES := $(shell find ./ -type f -name '*.c')
OBJ    := $(CFILES:.c=.o)

.PHONY: all clean

all: $(KERNEL)

stivale2.h:
	wget https://github.com/stivale/stivale/raw/master/stivale2.h

$(KERNEL): $(OBJ)
	$(LD) $(OBJ) $(LDFLAGS) $(INTERNALLDFLAGS) -o $@

%.o: %.c stivale2.h
	$(CC) $(CFLAGS) $(INTERNALCFLAGS) -c $< -o $@

clean:
	rm -rf $(KERNEL) $(OBJ)
