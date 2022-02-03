<div align="center">

# KnutOS

#### _An x86_64 hobbyist OS made for FUN and LEARNING! It is written in C and Assembly and features a monolithic kernel._

</div><br/>

## Screenshots

<img src="https://github.com/Tix3Dev/KnutOS/blob/main/screenshots/KnutOS%20silent%20boot.png"> </a>
*⤷ Silent boot*

<img src="https://github.com/Tix3Dev/KnutOS/blob/main/screenshots/KnutOS%20verbose%20boot.png"> </a>
*⤷ Verbose boot*

<img src="https://github.com/Tix3Dev/KnutOS/blob/main/screenshots/KnutOS%20shell.png"> </a>
*⤷ Shell*

<img src="https://github.com/Tix3Dev/KnutOS/blob/main/screenshots/KnutOS%20interrupt%20screen.png"> </a>
*⤷ Interrupt screen*

<img src="https://github.com/Tix3Dev/KnutOS/blob/main/screenshots/KnutOS%20serial%20communication.png"> </a>
*⤷ Local terminal showing serial communication*

## Features

- Limine bootloader
- Stivale2 boot protocol
- GDT
- TSS
- IDT & ISR's
- Physical memory manager -> Bitmap based
- Virtual memory manager -> 4- & 5-level paging
- Slab allocator
- Serial communication
- Basic framebuffer
- Basic keyboard driver with a small shell
- Basic kernel libraries

## Future Features

- C standard library port
- ELF loader
- IPC support
- SMP
- Double buffering
- Compositor
- GUI library

## Dependencies

**NOTE: Depending on your Linux Distribution the package names might be different (supposing you are using Linux)**

- x86_64-elf-binutils
- x86_64-elf-gcc
- nasm-git
- make
- xorriso
- qemu

## Installation

- Install the dependencies
- Get the repository
  - git clone https://github.com/Tix3Dev/KnutOS --recursive
  - cd KnutOS
- Build it
  - make all
- Run it
  - make run

## Contributing

For those interested in contributing, please refer to the [contributing guidelines](https://github.com/Tix3Dev/KnutOS/blob/main/CONTRIBUTING.md).
