/*
    mouse.cpp - mouse driver class for x86 OS kernel
*/

#include "drivers/mouse.h"

using namespace morphios::common;
using namespace morphios::drivers;
using namespace morphios::kernel;

// Constructor - set cursor to centre of screen
MouseEventHandler::MouseEventHandler() {}

MouseEventHandler::~MouseEventHandler() {}

void MouseEventHandler::onMouseActivate() {
    x = SCREEN_W/2;
    y = SCREEN_H/2;
    // Display cursor on screen
    switchFGandBGcolours();
}

void MouseEventHandler::onMouseDown(common::uint8_t button) {}

void MouseEventHandler::onMouseUp(common::uint8_t button) {}

// Triggered when mouse move generates an interrupt
void MouseEventHandler::onMouseMove(int32_t xOffset, int32_t yOffset) {
    // Switch original fg and bg colours back
    switchFGandBGcolours();
    
    x += xOffset;
    y += yOffset;

    // Display new cursor location
    switchFGandBGcolours();    
}

void MouseEventHandler::switchFGandBGcolours() {
    // Stop cursor at boundary
    if (x >= SCREEN_W) x = SCREEN_W - 1;
    if (x < 0) x = 0;

    if (y >= SCREEN_H) y = SCREEN_H - 1;
    if (y < 0) y = 0;

    VGA::terminal_buffer[SCREEN_W*y + x] = 
          (VGA::terminal_buffer[SCREEN_W*y + x] & 0x0F00) << 4
        | (VGA::terminal_buffer[SCREEN_W*y + x] & 0xF000) >> 4
        | (VGA::terminal_buffer[SCREEN_W*y + x] & 0x00FF);
}

// Constructor
MouseDriver::MouseDriver(interruptsHandler* IRQhandler, MouseEventHandler *eventhandler) 
    : interruptHandle(IRQhandler, HW_INTERRUPT_OFFSET + 0x0C),
    dataPort(0x60),
    commandPort(0x64) {
    this->eventHandler = eventhandler;
}

// Enable interrupts and initialise the cursor to the centre of the screen
void MouseDriver::activate() {
    offset = 0;
    buttons = 0;
    
    // Activate event handler
    if (eventHandler != 0)
        eventHandler->onMouseActivate();        

    // Aux Input / IRQ12 Enable
    commandPort.write(0xA8); // Aux Input Enable Command
    commandPort.write(0x20); // Get Compaq Status Byte command
    uint8_t status = dataPort.read();
    status |= 0x2; // Set bit 1
    status &= ~(0x1 << 5); // Clear bit 5
    commandPort.write(0x60);
    dataPort.write(status);

    // Enable packet streaming
    commandPort.write(0xD4);
    dataPort.write(0xF4);
    dataPort.read();
}

// Destructor
MouseDriver::~MouseDriver() {}

// Handle the mouse IRQ
uint32_t MouseDriver::ISR(uint32_t esp) {        
    // Check status
    uint8_t status = commandPort.read();
    if (!(status & 0x20))
        return esp;
    
    buffer[offset] = dataPort.read();
    offset = (offset + 1) % 3;

    // Trigger onMouseMove() event  if buffer is filled
    if (offset == 0) {
        if (buffer[1] != 0 || buffer[2] != 0) {     
            // Ensure arguments are typecast to signed int8       
            eventHandler->onMouseMove((int8_t)buffer[1], -(int8_t)buffer[2]);
        }

        for (uint8_t i = 0; i < 3; i++) {
            if ((buffer[0] & (0x1 << i)) != (buttons & (0x1 << i))) {
                if (buttons & (0x1 << i))
                    eventHandler->onMouseUp(i + 1);
                else
                    eventHandler->onMouseDown(i + 1);
            }
        }
        buttons = buffer[0];
    }

    return esp;
}

