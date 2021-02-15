GPP_PARAMS = -m32
AS_PARAMS = --32
LD_PARAMS = -melf_i386

objects = loader.o kernel.o

%.o: %.cpp
	g++ $(GPP_PARAMS) -o $@ -c $<

%.o: %.s
	as $(AS_PARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	ld $(LD_PARAMS) -T $< -o $@ $(objects)

install: mykernel.bin
	sudo cp $< /boot/mykernel.bin
