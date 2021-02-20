
#include "common.h"

void kprintf(const char* data) {
    VGA vga = VGA();
	vga.terminal_write(data, strlen(data));
}
