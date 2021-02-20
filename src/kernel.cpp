/*
    kernel.cpp : x86 kernel written in C++

    Based on the "bare Bones" tutorial from https://wiki.osdev.org/Bare_Bones.

    Created by Syed Taqi Haider
*/

#include "vga.h"
#include "common.h"
#include "types.h"
#include "gdt.h"
#include "port.h"
#include "interrupts.h"
#include "keyboard.h"
#include "mouse.h"

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

void print_welcome_msg() {

	VGA vga;

	kprintf("                               __    _ ____  _____\n");
	kprintf("   ____ ___  ____  _________  / /_  (_) __ \\/ ___/\n");
	kprintf("  / __ `__ \\/ __ \\/ ___/ __ \\/ __ \\/ / / / /\\__ \\ \n");
	kprintf(" / / / / / / /_/ / /  / /_/ / / / / / /_/ /___/ / \n");
	kprintf("/_/ /_/ /_/\\____/_/  / .___/_/ /_/_/\\____//____/  \n");
	kprintf("                    /_/                           \n");
	kprintf("\nWake up, Neo\n");
	kprintf("The Matrix has you\nFollow the white rabbit\n...\nKnock, Knock, Neo.\n");
	kprintf("\n$ ");
	vga.isWelcome = false;
}

// kernel main function called by loader.s
extern "C" void kernel_main(void) 
{
	/* Initialize terminal interface */
	VGA vga;
	vga.terminal_initialize();

	print_welcome_msg();
 
	GlobalDescriptorTable gdt;
	interruptsHandler interrupts(&gdt);
	
	// NOTE: always initialise mouse before keyboard
	Mouse mouse(&interrupts);
	Keyboard keyboard(&interrupts);

    interrupts.Activate();

	while(1);
}
