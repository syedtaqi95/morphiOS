/*
    port.h - C++ header for 8-bit, 16-bit and 32-bit ports in x86 architecture
*/

#ifndef PORT_H
#define PORT_H

#include "common/types.h"

namespace morphios {
namespace kernel {

class Port {
    protected:
        morphios::common::uint16_t portNumber;
        Port(morphios::common::uint16_t portNumber);
        ~Port(); // 
};

class Port8Bit : public Port {
    public:
        Port8Bit(morphios::common::uint16_t portNumber);
        ~Port8Bit();
        virtual void write(morphios::common::uint8_t data);
        virtual morphios::common::uint8_t read();
};

class Port16Bit : public Port {
    public:
        Port16Bit(morphios::common::uint16_t portNumber);
        ~Port16Bit();
        virtual void write(morphios::common::uint16_t data);
        virtual morphios::common::uint16_t read();
};

class Port32Bit : public Port {
    public:
        Port32Bit(morphios::common::uint16_t portNumber);
        ~Port32Bit();
        virtual void write(morphios::common::uint32_t data);
        virtual morphios::common::uint32_t read();
};

} // namespace kernel
} // namespace morphios

#endif // PORT_H