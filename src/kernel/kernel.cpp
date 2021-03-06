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
#include "gui/desktop.h"
#include "gui/window.h"
#include "gui/render.h"
#include "kernel/multithreading.h"

// Uncomment to use graphics mode. Otherwise boots up in text mode
// #define GRAPHICS_MODE

// Namespaces
using namespace morphios::common;
using namespace morphios::kernel;
using namespace morphios::drivers;
using namespace morphios::gui;

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This code will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This kernel needs to be compiled with a ix86-elf compiler"
#endif

// Example tasks
void taskA() {
	while(1)
		kprintf("A");
}

void taskB() {
	while(1)
		kprintf("B");
}

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
 
	// Setup GDT
	GlobalDescriptorTable gdt;

	// Setup multithreading
	TaskManager taskManager;
	Task task1(&gdt, taskA);
	taskManager.addTask(&task1);
	Task task2(&gdt, taskB);
	taskManager.addTask(&task2);

	// Setup IDT / interrupts
	interruptsHandler interrupts(&gdt, &taskManager);

	#ifdef GRAPHICS_MODE
		// Initialise desktop
		Desktop desktop(320, 200, 0, 0, 0xA8);
	#endif
	
	// Initialise drivers using DriverManager class
	DriverManager drvManager;

	// Add mouse driver
	// NOTE: always initialise mouse before keyboard as they are on the same I/O port
	#ifdef GRAPHICS_MODE
		MouseDriver mouse(&interrupts, &desktop);
	#else
		MouseEventHandler mouseEventH;
		MouseDriver mouse(&interrupts, &mouseEventH);
	#endif	
	drvManager.addDriver(&mouse);

	// Add keyboard driver
	#ifdef GRAPHICS_MODE
		KeyboardDriver keyboard(&interrupts, &desktop);
	#else
		KeyboardEventHandler keyboardEventH;
		KeyboardDriver keyboard(&interrupts, &keyboardEventH);
	#endif	
	drvManager.addDriver(&keyboard);

	// Activate PCI controller
	PCIController PCI;
	PCI.findDevices(&drvManager);

	// Activate drivers
	drvManager.activateAll();

	#ifdef GRAPHICS_MODE
		vga.setMode(320, 200, 8);
		//make new window and attach it to the desktop
		Window win1(&desktop, 10, 10, 20, 20, 0xA8, 0x00, 0x00);
		desktop.addChild(&win1);
		Window win2(&desktop, 40, 15, 30, 30, 0x00, 0xA8, 0x00);
		desktop.addChild(&win2);
		// Create rendering frame
		Render render;
	#else
		// Start shell
		kprintf("$ ");
		VGA::isWelcome = false;
	#endif

	// Activate interrupts
    interrupts.Activate();

	while(1) {
		#ifdef GRAPHICS_MODE
            desktop.draw(&render);
			// Display rendered frame
			render.display(&vga);
        #endif
	}
}
