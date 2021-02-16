# Use g++ i686 Cross Compiler
GCC_FOLDER = ~/GCC_SRC
C = $(GCC_FOLDER)/bin/i686-elf-gcc
AS = $(GCC_FOLDER)/bin/i686-elf-as
LD = $(C)

# Compiler flags
C_FLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -v
LD_FLAGS = -ffreestanding -O2 -nostdlib -lgcc

# Objects
objects = boot.o kernel.o

# Targets
%.o : %.s
	$(AS) boot.s -o boot.o

%.o : %.c
	$(C) -o $@ -c $< $(C_FLAGS)

myos.bin: linker.ld $(objects)
	$(LD) -T $< -o $@ $(LD_FLAGS) $(objects)
	# i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

install: myos.bin
	mkdir -p isodir/boot/grub
	cp myos.bin isodir/boot/myos.bin
	echo 'set timeout=0'                      > isodir/boot/grub/grub.cfg
	echo 'set default=0'                     >> isodir/boot/grub/grub.cfg
	echo ''                                  >> isodir/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> isodir/boot/grub/grub.cfg
	echo '  multiboot /boot/myos.bin'    >> isodir/boot/grub/grub.cfg
	echo '}'                                 >> isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir
	rm -rf isodir

clean:
	rm -f $(objects) myos.bin myos.iso isodir

run:
	qemu-system-i386 -cdrom myos.iso
