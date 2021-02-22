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

namespace morphios {
namespace drivers {

static const morphios::common::uint8_t KEY_RELEASE_OFFSET = 0x80; // Add this offset to get the value of a key release

// Figures out what to do with a keyboard event
class KeyboardEventHandler {
protected:
    char currentChar; // ASCII value of last key press
    bool isShiftPressed;
    bool isControlPressed;
    bool isAltPressed;
    bool isCapsLockOn;
    
    void getASCIIChar(morphios::common::uint8_t scanCode);
    char getCharacter();
    void printCharacter();

public:
    KeyboardEventHandler();
    ~KeyboardEventHandler();
    
    virtual void onKeyUp(morphios::common::uint8_t scanCode);
    virtual void onKeyDown(morphios::common::uint8_t scanCode);
};

// Keyboard driver class derived from Driver parent class
class KeyboardDriver : public morphios::kernel::interruptHandle, public Driver {
protected:
    morphios::kernel::Port8Bit dataPort;
    morphios::kernel::Port8Bit commandPort;
    KeyboardEventHandler *eventHandler;

    morphios::common::uint8_t scanCode; // Scan code of current key press

public:
    KeyboardDriver(morphios::kernel::interruptsHandler* IRQhandler, KeyboardEventHandler *eventHandler);
    ~KeyboardDriver();    
    virtual morphios::common::uint32_t ISR(morphios::common::uint32_t esp);
    virtual void activate();
};

} // namespace drivers
} // namespace morphios


#endif // KEYBOARD_H