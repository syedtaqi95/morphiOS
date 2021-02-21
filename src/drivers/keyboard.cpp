/*
    keyboard.cpp - Keyboard driver class for x86 OS kernel
*/

#include "drivers/keyboard.h"

KeyboardEventHandler::KeyboardEventHandler() {
    // Initialise key variables (pun intended)
    currentChar = '\0';
    scanCode = 0;
    isShiftPressed = false;
    isControlPressed = false;
    isCapsLockOn = false;
}
KeyboardEventHandler::~KeyboardEventHandler() {

}

void KeyboardEventHandler::onKeyUp() {

}
void KeyboardEventHandler::onKeyDown(uint8_t) {
    getASCIIChar();   

    // Print to stdout
    char *msg = " ";
    msg[0] = currentChar;
    kprintf(msg);
}

char KeyboardEventHandler::getCharacter() {
    return currentChar;
}

uint8_t KeyboardEventHandler::getScancode() {
    return scanCode;
}

void KeyboardEventHandler::printCharacter() {
    char* msg = " ";
    msg[0] = currentChar;
    kprintf(msg);
}

// Convert key press to ASCII character (UK layout - uses scan code 1)
// Accounts for shift, control, alt and capsLock
// TODO : implement as a LUT - for uppercase add ASCII offset to lowercase
void KeyboardEventHandler::getASCIIChar() {
    switch(scanCode) {
        case 0x01:
            currentChar = (char)0x1B; break;
        case 0x02:
            currentChar = '1';
            if(isShiftPressed)
                currentChar = '!';
            break;
        case 0x03:
            currentChar = '2'; 
            if(isShiftPressed)
                currentChar = '"';
            break;
        case 0x04:
            currentChar = '3'; 
            if(isShiftPressed)
                currentChar = '£';
            break;
        case 0x05:
            currentChar = '4'; 
            if(isShiftPressed)
                currentChar = '$';
            break;
        case 0x06:
            currentChar = '5'; 
            if(isShiftPressed)
                currentChar = '%';
            break;
        case 0x07:
            currentChar = '6'; 
            if(isShiftPressed)
                currentChar = '^';
            break;
        case 0x08:
            currentChar = '7'; 
            if(isShiftPressed)
                currentChar = '&';
            break;
        case 0x09:
            currentChar = '8'; 
            if(isShiftPressed)
                currentChar = '*';
            break;
        case 0x0A:
            currentChar = '9'; 
            if(isShiftPressed)
                currentChar = '(';
            break;
        case 0x0B:
            currentChar = '0'; 
            if(isShiftPressed)
                currentChar = ')';
            break;
        case 0x0C:
            currentChar = '-'; 
            if(isShiftPressed)
                currentChar = '_';
            break;
        case 0x0D:
            currentChar = '='; 
            if(isShiftPressed)
                currentChar = '+';
            break;
        case 0x0E:
            currentChar = '\b'; break;
        case 0x0F:
            currentChar = ' '; break; // TODO: output actual tab
        case 0x10:
            currentChar = 'q'; 
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'Q';
            break;
        case 0x11:
            currentChar = 'w'; 
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'W';
            break;
        case 0x12:
            currentChar = 'e'; 
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'E';
            break;
        case 0x13:
            currentChar = 'r'; 
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'R';
            break;
        case 0x14:
            currentChar = 't'; 
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'T';
            break;
        case 0x15:
            currentChar = 'y'; 
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'Y';
            break;
        case 0x16:
            currentChar = 'u'; 
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'U';
            break;
        case 0x17:
            currentChar = 'i'; 
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'I';
            break;
        case 0x18:
            currentChar = 'o'; 
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'O';
            break;
        case 0x19:
            currentChar = 'p'; 
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'P';
            break;
        case 0x1A:
            currentChar = '['; 
            if(isShiftPressed)
                currentChar = '{';
            break;
        case 0x1B:
            currentChar = ']'; 
            if(isShiftPressed)
                currentChar = '}';
            break;
        case 0x1C:
            currentChar = '\n'; break;
        case 0x1D:
            isControlPressed = true; break;
        case 0x1E:
            currentChar = 'a'; 
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'A';
            break;
        case 0x1F:
            currentChar = 's'; 
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'S';
            break;
        case 0x20:
            currentChar = 'd';
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'D';
            break;
        case 0x21:
            currentChar = 'f'; 
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'F';
            break;
        case 0x22:
            currentChar = 'g';
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'G';
            break;
        case 0x23:
            currentChar = 'h'; 
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'H';
            break;
        case 0x24:
            currentChar = 'j'; 
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'J';
            break;
        case 0x25:
            currentChar = 'k'; 
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'K';
            break;
        case 0x26:
            currentChar = 'l'; 
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'L';
            break;
        case 0x27:
            currentChar = ';'; 
            if(isShiftPressed)
                currentChar = ':';
            break;
        case 0x28:
            currentChar = '\''; 
            if(isShiftPressed)
                currentChar = '|';
            break;
        case 0x2A:
            isShiftPressed = true; break;
        case 0x2B:
            currentChar = '#'; 
            if(isShiftPressed)
                currentChar = '~';
            break;
        case 0x2C:
            currentChar = 'z'; 
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'Z';
            break;
        case 0x2D:
            currentChar = 'x'; 
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'X';
            break;
        case 0x2E:
            currentChar = 'c'; 
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'C';
            break;
        case 0x2F:
            currentChar = 'v'; 
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'V';
            break;
        case 0x30:
            currentChar = 'b'; 
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'B';
            break;
        case 0x31:
            currentChar = 'n'; 
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'N';
            break;
        case 0x32:
            currentChar = 'm'; 
            if(isShiftPressed || isCapsLockOn)
                currentChar = 'M';
            break;
        case 0x33:
            currentChar = ','; 
            if(isShiftPressed)
                currentChar = '<';
            break;
        case 0x34:
            currentChar = '.'; 
            if(isShiftPressed)
                currentChar = '>';
            break;
        case 0x35:
            currentChar = '/'; 
            if(isShiftPressed)
                currentChar = '?';
            break;
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
KeyboardDriver::KeyboardDriver(interruptsHandler* IRQhandler, KeyboardEventHandler *eventHandler) 
    : interruptHandle(IRQhandler, HW_INTERRUPT_OFFSET + 0x01),
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
}

// Destructor
KeyboardDriver::~KeyboardDriver() {}

// Handle the keyboard IRQ
uint32_t KeyboardDriver::ISR(uint32_t esp) {

    scanCode = dataPort.read();    
    return esp;
}




