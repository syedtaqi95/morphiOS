/*
    common.h - common declarations used in the OS
*/

#ifndef COMMON_H
#define COMMON_H

#include "common/types.h"
#include "drivers/vga.h"

namespace morphios {
namespace common {

void kprintf(const char* str);

void kprintHex(uint8_t hexVal);

size_t strlen(const char* str);

} // namespace common
} // namespace morphios

#endif // COMMON_H