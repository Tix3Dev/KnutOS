<div align="center">

# KnutOS

#### _An x86_64 hobbyist OS made for FUN and LEARNING! It is written in C and Assembly and features a monolithic kernel._

</div><br/>

## Screenshots

<img src="https://github.com/Tix3Dev/KnutOS/blob/main/screenshots/KnutOS%20silent%20boot.png"> </a>
<img src="https://github.com/Tix3Dev/KnutOS/blob/main/screenshots/KnutOS%20verbose%20boot.png"> </a>
<img src="https://github.com/Tix3Dev/KnutOS/blob/main/screenshots/KnutOS%20shell.png"> </a>
<img src="https://github.com/Tix3Dev/KnutOS/blob/main/screenshots/KnutOS%20interrupt%20screen.png"> </a>

## Features

- Limine bootloader
- Stivale2 boot protocol
- GDT
- TSS
- IDT & ISR's
- Physical memory manager -> Bitmap based
- Virtual memory manager -> 4- & 5-level paging
- Serial communication
- Basic framebuffer
- Basic keyboard driver with a small shell
- Basic kernel libraries

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
