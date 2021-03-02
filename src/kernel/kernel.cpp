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
#include "drivers/driver.h"
#include "drivers/pci.h"
#include "drivers/vga.h"
#include "drivers/keyboard.h"
#include "drivers/mouse.h"

// Namespaces
using namespace morphios::common;
using namespace morphios::kernel;
using namespace morphios::drivers;


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
 
	// Setup GDT and IDT
	GlobalDescriptorTable gdt;
	interruptsHandler interrupts(&gdt);
	
	// Initialise drivers using DriverManager class
	DriverManager drvManager;

	// Add mouse driver
	// NOTE: always initialise mouse before keyboard as they are on the same I/O port
	MouseEventHandler mouseEventH;
	MouseDriver mouse(&interrupts, &mouseEventH);
	drvManager.addDriver(&mouse);

	// Add keyboard driver
	KeyboardEventHandler keyboardEventH;
	KeyboardDriver keyboard(&interrupts, &keyboardEventH);
	drvManager.addDriver(&keyboard);

	// Activate PCI controller
	PCIController PCI;
	PCI.findDevices(&drvManager);

	// Activate drivers
	drvManager.activateAll();

	// Activate interrupts
    interrupts.Activate();

	// Start shell
	kprintf("$ ");
	VGA::isWelcome = false;

	// Activate VGA 320x200x8 graphics mode
	vga.setMode(320, 200, 8);
	
	// Draw a blue rectangle
	for(int32_t y = 0; y < 200; y++) {
		for(int32_t x = 0; x < 320; x++) {
			vga.putPixel(x, y, 0, 0, 0xA8);
		}
	}

	// Draw a line
	vga.putLine(10, 10, 120, 140, 0, 0, 0);

	while(1);
}
