/*
    kernel.cpp : x86 kernel written in C++

    Based on the "bare Bones" tutorial from https://wiki.osdev.org/Bare_Bones.

    Created by Syed Taqi Haider
*/

// Includes
#include "common/types.h"
#include "common/common.h"
#include "kernel/gdt.h"
#include "kernel/port.h"
#include "kernel/interrupts.h"
#include "drivers/vga.h"
#include "drivers/keyboard.h"
#include "drivers/mouse.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This code will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This kernel needs to be compiled with a ix86-elf compiler"
#endif

// Constructors
typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
    	(*i)();
}



// kernel main function called by loader.s
extern "C" void kernel_main(void) 
{
	/* Initialize terminal interface */
	VGA vga;
	vga.terminal_initialize();
	vga.print_welcome_msg();
 
	GlobalDescriptorTable gdt;
	interruptsHandler interrupts(&gdt);
	
	// NOTE: always initialise mouse before keyboard as they share the same port
	MouseDriver mouse(&interrupts);
	KeyboardDriver keyboard(&interrupts);

    interrupts.Activate();

	while(1);
}
