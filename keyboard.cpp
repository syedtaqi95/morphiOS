/*
    keyboard.cpp - Keyboard driver class for x86 OS kernel
*/

#include "keyboard.h"

// Constructor
Keyboard::Keyboard(interruptsHandler* handler) 
    : interruptHandle(handler, 0x21),
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

    // Initialise current character and scan code
    currentChar = '\0';
    currentScancode = 0;
}

// Destructor
Keyboard::~Keyboard() {}

// Handle the keyboard IRQ
uint32_t Keyboard::ISR(uint32_t esp) {

    uint8_t key = dataPort.read();

    if (key < 0x80) {
        
    }

    return esp;
}


char Keyboard::getCharacter() {
    return currentChar;
}

uint8_t Keyboard::getScancode() {
    return currentScancode;
}

void Keyboard::printCharacter() {
    char* msg = " ";
    msg[0] = currentChar;
    kprintf(msg);
}



