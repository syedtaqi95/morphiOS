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
static const uint8_t KEY_RELEASE_OFFSET = 0x80; // Add this offset to get the value of a key release

// Keyboard event handler class
class KeyboardEventHandler {
protected:
    char currentChar; // ASCII value of last key press
    uint8_t scanCode; // Scan code of current key press
    bool isShiftPressed;
    bool isControlPressed;
    bool isAltPressed;
    bool isCapsLockOn;
    
    void getASCIIChar();

public:
    KeyboardEventHandler();
    ~KeyboardEventHandler();
    
    virtual void onKeyUp();
    virtual void onKeyDown();
    
    char getCharacter();
    uint8_t getScancode();
    void printCharacter();
};

// Keyboard driver class derived from Driver parent class
class KeyboardDriver : public interruptHandle, public Driver {
protected:
    Port8Bit dataPort;
    Port8Bit commandPort;
    KeyboardEventHandler *eventHandler;
    
    char currentChar; // ASCII value of last key press
    uint8_t scanCode; // Scan code of current key press

public:
    KeyboardDriver(interruptsHandler* IRQhandler, KeyboardEventHandler *eventHandler);
    ~KeyboardDriver();
    
    virtual uint32_t ISR(uint32_t esp);
};


#endif // KEYBOARD_H