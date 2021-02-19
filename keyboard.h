/*
    keyboard.h - Keyboard driver for x86 OS kernel
*/

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "types.h"
#include "port.h"
#include "interrupts.h"

class Keyboard : public interruptHandle {
protected:
    Port8Bit KeyboardDataport;
    Port8Bit KeyboardCommandPort;
    char currentChar;

public:
    Keyboard(interruptsHandler* handler);
    ~Keyboard();
    char readCharacter();
    void printCharacter();
    virtual uint32_t ISR(uint32_t esp);
};


#endif // KEYBOARD_H