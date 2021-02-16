/*
    port.h - C++ header for 8-bit, 16-bit and 32-bit ports in x86 architecture
*/

#ifndef PORT_H
#define PORT_H

#include "types.h"

class Port {
    protected:
        uint16_t portNumber;
        Port(uint16_t portNumber);
        ~Port();
};

class Port8Bit : public Port {
    Port8Bit(uint16_t portNumber);
    ~Port8Bit();
    virtual void write(uint8_t data);
    virtual uint8_t read();
};

class Port16Bit : public Port {
    Port16Bit(uint16_t portNumber);
    ~Port16Bit();
    virtual void write(uint16_t data);
    virtual uint16_t read();
};

class Port32Bit : public Port {
    Port32Bit(uint16_t portNumber);
    ~Port32Bit();
    virtual void write(uint32_t data);
    virtual uint32_t read();
};

#endif // PORT_H