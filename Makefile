# Use the i686 Cross Compiler
GCC_FOLDER := ~/GCC_SRC
C := $(GCC_FOLDER)/bin/i686-elf-gcc
CXX := $(GCC_FOLDER)/bin/i686-elf-g++
AS := $(GCC_FOLDER)/bin/i686-elf-as
LD := $(CXX)

# Compiler flags
C_FLAGS := -std=gnu99 -ffreestanding -O2 -Wall -Wextra -v
CXX_FLAGS := -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
AS_FLAGS := --32
LD_FLAGS := -ffreestanding -O2 -nostdlib -lgcc

# Directories
OBJ_DIR := obj
SRC_DIR := src

# Objects
objects = 	$(OBJ_DIR)/boot/boot.o \
			$(OBJ_DIR)/common/common.o \
			$(OBJ_DIR)/kernel/gdt.o \
			$(OBJ_DIR)/drivers/driver.o \
			$(OBJ_DIR)/kernel/port.o \
			$(OBJ_DIR)/kernel/interrupts-stubs.o \
			$(OBJ_DIR)/kernel/interrupts.o \
			$(OBJ_DIR)/drivers/keyboard.o \
			$(OBJ_DIR)/drivers/mouse.o \
			$(OBJ_DIR)/drivers/vga.o \
			$(OBJ_DIR)/kernel/kernel.o

# Targets
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.s
	mkdir -p $(@D)
	$(AS) $< -o $@ $(AS_FLAGS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(C) -o $@ -c $< $(C_FLAGS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) -o $@ -c $< $(CXX_FLAGS)

morphios.bin: linker.ld $(objects)
	$(LD) -T $< -o $@ $(LD_FLAGS) $(objects)

install: morphios.bin
	mkdir -p isodir/boot/grub
	cp morphios.bin isodir/boot/morphios.bin
	echo 'set timeout=0'                      > isodir/boot/grub/grub.cfg
	echo 'set default=0'                     >> isodir/boot/grub/grub.cfg
	echo ''                                  >> isodir/boot/grub/grub.cfg
	echo 'menuentry "morphiOS" {' >> isodir/boot/grub/grub.cfg
	echo '  multiboot /boot/morphios.bin'    >> isodir/boot/grub/grub.cfg
	echo '}'                                 >> isodir/boot/grub/grub.cfg
	grub-mkrescue -o morphios.iso isodir
	rm -rf isodir

.PHONY: clean run
clean:
	rm -rf morphios.bin morphios.iso isodir $(OBJ_DIR)

run:
	qemu-system-i386 -cdrom morphios.iso
