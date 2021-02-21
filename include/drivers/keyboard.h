/*
    keyboard.h - Keyboard driver for x86 OS kernel
*/

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "common/types.h"
#include "drivers/driver.h"
#include "kernel/port.h"
#include "kernel/interrupts.h"
#include "common/common.h"

static const size_t NUM_ASCII_VALUES = 256; // ASCII array size
static const uint8_t KEY_RELEASE_OFFSET = 0x80; // Value to add to get the value of a key release



class KeyboardDriver : public interruptHandle, public Driver {
protected:
    Port8Bit dataPort;
    Port8Bit commandPort;
    
    char currentChar; // ASCII value of last key press
    uint8_t scanCode; // Scan code of current key press
    
    bool isShiftPressed;
    bool isControlPressed;
    bool isAltPressed;
    bool isCapsLockOn;

    void getASCIIChar();

public:
    KeyboardDriver(interruptsHandler* handler);
    ~KeyboardDriver();
    
    virtual uint32_t ISR(uint32_t esp);
    
    char getCharacter();
    uint8_t getScancode();
    void printCharacter();
};


#endif // KEYBOARD_H