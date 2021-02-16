# CustomOS

Developing my own 32-bit x86 (IA32) based operating system written in C++.

Used some of the code from the OSDev Wiki: https://wiki.osdev.org/

## Installation

### Dependencies

#### Essential:

- [make](https://www.gnu.org/software/make/)
- Host OS: Linux. Development has been done on [Windows Subsytem for Linux](https://docs.microsoft.com/en-us/windows/wsl/install-win10) on Windows 10 (Ubuntu 20.04.2 LTS). Or you can develop on Ubuntu or another Debian based OS.
- GCC cross compiler for ```i686-elf``` targets. Read [this tutorial](https://wiki.osdev.org/GCC_Cross-Compiler) for more info on how to do this.

#### Optional:

- Install [qemu](https://www.qemu.org/). Required for booting and testing the OS in a VM locally.


## Compiling customOS

Open a terminal and run:
- ```make install```


## Running customOS

You can run customOS locally (requires ```qemu```).
- ```make run```


