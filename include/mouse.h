/*
    mouse.h - mouse driver for x86 OS kernel
*/

#ifndef MOUSE_H
#define MOUSE_H

#include "types.h"
#include "port.h"
#include "interrupts.h"
#include "common.h"
#include "vga.h"

class Mouse : public interruptHandle {
protected:
    Port8Bit dataPort;
    Port8Bit commandPort;
    uint8_t buffer[3];
    uint8_t offset;
    uint8_t buttons;
    int8_t x, y;

public:
    Mouse(interruptsHandler* handler);
    ~Mouse();
    
    virtual uint32_t ISR(uint32_t esp);
};


#endif // MOUSE_H