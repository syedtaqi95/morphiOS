# Use the i686 Cross Compiler
GCC_FOLDER = ~/GCC_SRC
C = $(GCC_FOLDER)/bin/i686-elf-gcc
CXX = $(GCC_FOLDER)/bin/i686-elf-g++
AS = $(GCC_FOLDER)/bin/i686-elf-as
LD = $(CXX)

# Compiler flags
C_FLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -v
# CXX_FLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
CXX_FLAGS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
LD_FLAGS = -ffreestanding -O2 -nostdlib -lgcc

# Objects
objects = boot.o gdt.o port.o interrupts-stubs.o interrupts.o kernel.o

# Targets
%.o : %.s
	$(AS) $< -o $@

%.o : %.c
	$(C) -o $@ -c $< $(C_FLAGS)

%.o : %.cpp
	$(CXX) -o $@ -c $< $(CXX_FLAGS)

morphios.bin: linker.ld $(objects)
	$(LD) -T $< -o $@ $(LD_FLAGS) $(objects)

install: morphios.bin
	mkdir -p isodir/boot/grub
	cp morphios.bin isodir/boot/morphios.bin
	echo 'set timeout=0'                      > isodir/boot/grub/grub.cfg
	echo 'set default=0'                     >> isodir/boot/grub/grub.cfg
	echo ''                                  >> isodir/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> isodir/boot/grub/grub.cfg
	echo '  multiboot /boot/morphios.bin'    >> isodir/boot/grub/grub.cfg
	echo '}'                                 >> isodir/boot/grub/grub.cfg
	grub-mkrescue -o morphios.iso isodir
	rm -rf isodir

.PHONY: clean run
clean:
	rm -f $(objects) morphios.bin morphios.iso isodir

run:
	qemu-system-i386 -cdrom morphios.iso
