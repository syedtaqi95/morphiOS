/*
    keyboard.cpp - Keyboard driver class for x86 OS kernel
*/

#include "keyboard.h"

void Keyboard::getASCIIChar() {
    switch(scanCode) {
        case 0x01:
            currentChar = (char)0x1B; break;
        case 0x02:
            currentChar = '1'; break;
        case 0x03:
            currentChar = '2'; break;
        case 0x04:
            currentChar = '3'; break;
        case 0x05:
            currentChar = '4'; break;
        case 0x06:
            currentChar = '5'; break;
        case 0x07:
            currentChar = '6'; break;
        case 0x08:
            currentChar = '7'; break;
        case 0x09:
            currentChar = '8'; break;
        case 0x0A:
            currentChar = '9'; break;
        case 0x0B:
            currentChar = '0'; break;
        case 0x0C:
            currentChar = '-'; break;
        case 0x0D:
            currentChar = '='; break;
        case 0x0E:
            currentChar = '\b'; break;
        case 0x0F:
            currentChar = '\t'; break;
        case 0x10:
            currentChar = 'q'; break;
        case 0x11:
            currentChar = 'w'; break;
        case 0x12:
            currentChar = 'e'; break;
        case 0x13:
            currentChar = 'r'; break;
        case 0x14:
            currentChar = 't'; break;
        case 0x15:
            currentChar = 'y'; break;
        case 0x16:
            currentChar = 'u'; break;
        case 0x17:
            currentChar = 'i'; break;
        case 0x18:
            currentChar = 'o'; break;
        case 0x19:
            currentChar = 'p'; break;
        case 0x1A:
            currentChar = '['; break;
        case 0x1B:
            currentChar = ']'; break;
        case 0x1C:
            currentChar = '\n'; break;
        case 0x1D:
            isControlPressed = true; break;
        case 0x1E:
            currentChar = 'a'; break;
        case 0x1F:
            currentChar = 's'; break;
        case 0x20:
            currentChar = 'd'; break;
        case 0x21:
            currentChar = 'f'; break;
        case 0x22:
            currentChar = 'g'; break;
        case 0x23:
            currentChar = 'h'; break;
        case 0x24:
            currentChar = 'j'; break;
        case 0x25:
            currentChar = 'k'; break;
        case 0x26:
            currentChar = 'l'; break;
        case 0x27:
            currentChar = ';'; break;
        case 0x28:
            currentChar = '\''; break;
        case 0x2A:
            isShiftPressed = true; break;
        case 0x2B:
            currentChar = '#'; break;
        case 0x2C:
            currentChar = 'z'; break;
        case 0x2D:
            currentChar = 'x'; break;
        case 0x2E:
            currentChar = 'c'; break;
        case 0x2F:
            currentChar = 'v'; break;
        case 0x30:
            currentChar = 'b'; break;
        case 0x31:
            currentChar = 'n'; break;
        case 0x32:
            currentChar = 'm'; break;
        case 0x33:
            currentChar = ','; break;
        case 0x34:
            currentChar = '.'; break;
        case 0x35:
            currentChar = '/'; break;
        case 0x36:
            isShiftPressed = true; break;
        case 0x37:
            currentChar = '*'; break;
        case 0x38:
            isAltPressed = true; break;
        case 0x39:
            currentChar = ' '; break;
        case 0x3A:
            isCapsLockOn = !isCapsLockOn; break;
        case 0x47:
            currentChar = '7'; break;
        case 0x48:
            currentChar = '8'; break;
        case 0x49:
            currentChar = '9'; break;
        case 0x4A:
            currentChar = '-'; break;
        case 0x4B:
            currentChar = '4'; break;
        case 0x4C:
            currentChar = '5'; break;
        case 0x4D:
            currentChar = '6'; break;
        case 0x4E:
            currentChar = '+'; break;
        case 0x4F:
            currentChar = '1'; break;
        case 0x50:
            currentChar = '2'; break;
        case 0x51:
            currentChar = '3'; break;
        case 0x52:
            currentChar = '0'; break;
        case 0x53:
            currentChar = '.'; break;
        case 0x56:
            currentChar = '\\'; break;
        case 0x1D + KEY_RELEASE_OFFSET:
            isControlPressed = false; break;
        case 0x2A + KEY_RELEASE_OFFSET:
            isShiftPressed = false; break;
        case 0x36 + KEY_RELEASE_OFFSET:
            isShiftPressed = false; break;
        case 0x38 + KEY_RELEASE_OFFSET:
            isAltPressed = false; break;
        default:
            currentChar = '\0';
    }
}

// Constructor
Keyboard::Keyboard(interruptsHandler* handler) 
    : interruptHandle(handler, 0x21),
    dataPort(0x60),
    commandPort(0x64) {

    // Check status bit
    while(commandPort.read() & 0x01)
        dataPort.read();
    
    // Initialise reading key presses
    commandPort.write(0xAE); // Activate interrupts
    commandPort.write(0x20); // Command 0x20 = read controller command byte
    uint8_t status = (dataPort.read() | 1) & ~0x10;
    commandPort.write(0x60); // Command 0x60 = set controller command byte
    dataPort.write(status);
    dataPort.write(0xF4);

    // Initialise key variables (pun intended)
    currentChar = '\0';
    scanCode = 0;
    isShiftPressed = false;
    isControlPressed = false;
    isCapsLockOn = false;
}

// Destructor
Keyboard::~Keyboard() {}

// Handle the keyboard IRQ
uint32_t Keyboard::ISR(uint32_t esp) {

    scanCode = dataPort.read();
    getASCIIChar();   

    char *msg = " ";
    msg[0] = currentChar;
    kprintf(msg);
    
    return esp;
}


char Keyboard::getCharacter() {
    return currentChar;
}

uint8_t Keyboard::getScancode() {
    return scanCode;
}

void Keyboard::printCharacter() {
    char* msg = " ";
    msg[0] = currentChar;
    kprintf(msg);
}



