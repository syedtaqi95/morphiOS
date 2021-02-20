/*
	common.cpp - Common functions used in the project
*/

#include "common.h"

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void kprintf(const char* data) {
    VGA vga = VGA();
	vga.terminal_write(data, strlen(data));
}
