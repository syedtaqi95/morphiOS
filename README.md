# micrOS

Developing my own hobbyist 32-bit operating system written in C++ for the x86 architecture.

Used some of the code from the OSDev Wiki: https://wiki.osdev.org/

## Installation

### Dependencies

#### Essential:

- [make](https://www.gnu.org/software/make/)
- Host OS: Linux. Development has been done on [Windows Subsytem for Linux](https://docs.microsoft.com/en-us/windows/wsl/install-win10) on Windows 10 (Ubuntu 20.04.2 LTS). Or you can develop on Ubuntu or another Debian based OS.
- GCC cross compiler for ```i686-elf``` targets. Read [this tutorial](https://wiki.osdev.org/GCC_Cross-Compiler) for more info on how to do this.
- GRUB (specfically GRUB2) bootloader (to invoke ```grub-mkrescue```). Comes with Linux or google how to install it.
- Xorriso, the .iso creation engine used by grub-mkrescue (```sudo apt install xorriso```).

#### Optional:

- Install [qemu](https://www.qemu.org/). Required for booting and testing the OS in a VM locally.


## Compiling micrOS

Open a terminal and run:
- ```make install```


## Running micrOS

You can run micrOS locally using ```qemu```.
- ```make install```
- ```make run```

Or you can generate the ISO image and run it using Virtualbox.

## Cleaning the project

Open a terminal and run:
- ```make clean```

