/*
	common.cpp - Common functions used in the project
*/

#include "common/common.h"

using namespace morphios::common;
using namespace morphios::drivers;

size_t morphios::common::strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void morphios::common::kprintf(const char* data) {
    VGA vga;
	vga.terminal_write(data, strlen(data));
}

void morphios::common::kprintHex(uint8_t hexVal) {
	char *msg = "  ";
	char *hex = "0123456789ABCDEF";
	msg[0] = hex[(hexVal >> 4) & 0x0F];
	msg[1] = hex[hexVal & 0x0F];
	kprintf(msg);
}
