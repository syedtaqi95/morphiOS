/*
    mouse.cpp - Keyboard driver class for x86 OS kernel
*/

#include "mouse.h"

// Constructor
Mouse::Mouse(interruptsHandler* handler) 
    : interruptHandle(handler, HW_INTERRUPT_OFFSET + 0x0C),
    dataPort(0x60),
    commandPort(0x64) {

    // Check status bit
    while(commandPort.read() & 0x01)
        dataPort.read();
    
    // Initialise reading key presses
    commandPort.write(0xAE); // Activate interrupts
    commandPort.write(0x20); // Command 0x20 = read controller command byte
    uint8_t status = (dataPort.read() | 1) & ~0x10;
    commandPort.write(0x60); // Command 0x60 = set controller command byte
    dataPort.write(status);
    dataPort.write(0xF4);
}

// Destructor
Mouse::~Mouse() {}

// Handle the keyboard IRQ
uint32_t Mouse::ISR(uint32_t esp) {

    return esp;
}


