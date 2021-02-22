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

// Mouse event handler class
class MouseEventHandler {
protected:
    int8_t x, y;
    uint8_t buttonsStatus;
    // Typecast to signed int8
    static const int8_t SCREEN_W = (int8_t)VGA::VGA_WIDTH;
    static const int8_t SCREEN_H = (int8_t)VGA::VGA_HEIGHT;
    void switchFGandBGcolours();

public:
    MouseEventHandler();
    ~MouseEventHandler();
    virtual void onMouseActivate();
    virtual void onMouseDown();
    virtual void onMouseUp();
    virtual void onMouseMove(int8_t xOffset, int8_t yOffset);
};

class MouseDriver : public interruptHandle, public Driver {
protected:
    Port8Bit dataPort;
    Port8Bit commandPort;
    uint8_t buffer[3];
    uint8_t offset;
    MouseEventHandler *eventHandler;

public:
    MouseDriver(interruptsHandler *handler, MouseEventHandler *eventhandler);
    ~MouseDriver();    
    virtual uint32_t ISR(uint32_t esp);
    virtual void activate();
};


#endif // MOUSE_H