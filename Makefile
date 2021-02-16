# Use g++ i686 Cross Compiler
CXX = $(GCC_FOLDER)/bin/i686-elf-g++
AS = $(GCC_FOLDER)/bin/i686-elf-as
LD = $(GCC_FOLDER)/bin/i686-elf-ld

# Compiler flags
GPP_PARAMS = -m32 -Wno-write-strings -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
AS_PARAMS = --32
LD_PARAMS = -melf_i386
GCC_FOLDER = ~/GCC_SRC

# Objects
objects = loader.o kernel.o

# Targets
%.o: %.cpp
	$(CXX) $(GPP_PARAMS) -o $@ -c $<

%.o: %.s
	$(AS) $(AS_PARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	$(LD) $(LD_PARAMS) -T $< -o $@ $(objects)

install: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp mykernel.bin iso/boot/mykernel.bin
	echo 'set timeout=0'                      > iso/boot/grub/grub.cfg
	echo 'set default=0'                     >> iso/boot/grub/grub.cfg
	echo ''                                  >> iso/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin'    >> iso/boot/grub/grub.cfg
	echo '  boot'                            >> iso/boot/grub/grub.cfg
	echo '}'                                 >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=mykernel.iso iso
	rm -rf iso

clean:
	rm -f kernel.o loader.o mykernel.bin mykernel.iso