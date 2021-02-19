/*
    keyboard.h - Keyboard driver for x86 OS kernel
*/

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "types.h"
#include "port.h"
#include "interrupts.h"
#include "common.h"

class Keyboard : public interruptHandle {
protected:
    Port8Bit dataPort;
    Port8Bit commandPort;
    char currentChar; // Stores the value of the last key press
    uint8_t currentScancode;

public:
    Keyboard(interruptsHandler* handler);
    ~Keyboard();
    virtual uint32_t ISR(uint32_t esp);
    char getCharacter();
    uint8_t getScancode();
    void printCharacter();
};


#endif // KEYBOARD_H