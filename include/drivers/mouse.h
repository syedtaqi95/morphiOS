/*
    mouse.h - mouse driver for x86 OS kernel
*/

#ifndef MOUSE_H
#define MOUSE_H

#include "common/types.h"
#include "drivers/driver.h"
#include "kernel/port.h"
#include "kernel/interrupts.h"
#include "common/common.h"
#include "drivers/vga.h"

class MouseDriver : public interruptHandle, public Driver {
protected:
    Port8Bit dataPort;
    Port8Bit commandPort;
    uint8_t buffer[3];
    uint8_t offset;
    uint8_t buttons;
    int8_t x, y;

public:
    MouseDriver(interruptsHandler* handler);
    ~MouseDriver();
    
    virtual uint32_t ISR(uint32_t esp);
};


#endif // MOUSE_H