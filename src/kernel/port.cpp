/*
    port.cppC++ header for 8-bit, 16-bit and 32-bit ports in x86 architecture
*/

#include "kernel/port.h"

// Port constructor
Port::Port(uint16_t portNumber) {
    this->portNumber = portNumber;
}

// Port destructor
Port::~Port(){}

// Constructors and destructors of derived port classes
Port8Bit::Port8Bit(uint16_t portNumber) : Port(portNumber) {}
Port8Bit::~Port8Bit(){}
Port16Bit::Port16Bit(uint16_t portNumber) : Port(portNumber) {}
Port16Bit::~Port16Bit(){}
Port32Bit::Port32Bit(uint16_t portNumber) : Port(portNumber) {}
Port32Bit::~Port32Bit(){}

void Port8Bit::write(uint8_t data) {
    asm volatile("outb %0, %1" : : "a"(data), "Nd"(portNumber) );
}

uint8_t Port8Bit::read() {
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(portNumber) );
    return ret;
}

void Port16Bit::write(uint16_t data) {
    asm volatile("outw %0, %1" : : "a"(data), "Nd"(portNumber) );
}

uint16_t Port16Bit::read() {
    uint16_t ret;
    asm volatile ( "inw %1, %0"
                   : "=a"(ret)
                   : "Nd"(portNumber) );
    return ret;
}

void Port32Bit::write(uint32_t data) {
    asm volatile("outl %0, %1" : : "a"(data), "Nd"(portNumber) );
}

uint32_t Port32Bit::read() {
    uint32_t ret;
    asm volatile ( "inl %1, %0"
                   : "=a"(ret)
                   : "Nd"(portNumber) );
    return ret;
}
