/*
    mouse.cpp - Keyboard driver class for x86 OS kernel
*/

#include "mouse.h"

// Constructor
Mouse::Mouse(interruptsHandler* handler) 
    : interruptHandle(handler, HW_INTERRUPT_OFFSET + 0x0C),
    dataPort(0x60),
    commandPort(0x64) {

    int8_t SCREEN_W = (int8_t)VGA::VGA_WIDTH;
    int8_t SCREEN_H = (int8_t)VGA::VGA_HEIGHT;
    
    // Set the cursor to the centre of the screen
    offset = 0;
    buttons = 0;
    x = SCREEN_W/2;
    y = SCREEN_H/2;

    // swap fg and bg VGA colours at the cursor
    VGA::terminal_buffer[SCREEN_W*y + x] = 
          (VGA::terminal_buffer[SCREEN_W*y + x] & 0x0F00) << 4
        | (VGA::terminal_buffer[SCREEN_W*y + x] & 0xF000) >> 4
        | (VGA::terminal_buffer[SCREEN_W*y + x] & 0x00FF);
    
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
Mouse::~Mouse() {}

// Handle the keyboard IRQ
uint32_t Mouse::ISR(uint32_t esp) {

    uint8_t status = commandPort.read();
    if (!(status & 0x20))
        return esp;
    
    buffer[offset] = dataPort.read();
    offset = (offset + 1) % 3;

    int8_t SCREEN_W = (int8_t)VGA::VGA_WIDTH;
    int8_t SCREEN_H = (int8_t)VGA::VGA_HEIGHT;

    if (offset == 0) {
        if (buffer[1] != 0 || buffer[2] != 0) {
            
            VGA::terminal_buffer[SCREEN_W*y + x] = 
                  (VGA::terminal_buffer[SCREEN_W*y + x] & 0x0F00) << 4
                | (VGA::terminal_buffer[SCREEN_W*y + x] & 0xF000) >> 4
                | (VGA::terminal_buffer[SCREEN_W*y + x] & 0x00FF);
            
            // Stop cursor at boundary
            x += buffer[1];
            if (x >= SCREEN_W) x = SCREEN_W - 1;
            if (x < 0) x = 0;

            y -= buffer[2];
            if (y >= SCREEN_H) y = SCREEN_H - 1;
            if (y < 0) y = 0;

            VGA::terminal_buffer[SCREEN_W*y + x] = 
                  (VGA::terminal_buffer[SCREEN_W*y + x] & 0x0F00) << 4
                | (VGA::terminal_buffer[SCREEN_W*y + x] & 0xF000) >> 4
                | (VGA::terminal_buffer[SCREEN_W*y + x] & 0x00FF);
        }
    }

    // kprintf("M ");

    return esp;
}


