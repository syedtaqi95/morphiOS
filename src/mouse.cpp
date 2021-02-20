/*
    mouse.cpp - Keyboard driver class for x86 OS kernel
*/

#include "mouse.h"

// Constructor
Mouse::Mouse(interruptsHandler* handler) 
    : interruptHandle(handler, HW_INTERRUPT_OFFSET + 0x0C),
    dataPort(0x60),
    commandPort(0x64) {
    
    // Do something
}

// Destructor
Mouse::~Mouse() {}

// Handle the keyboard IRQ
uint32_t Mouse::ISR(uint32_t esp) {

    return esp;
}


