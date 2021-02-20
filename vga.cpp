/*
    vga.cpp - VGA class implementation to communicate with stdout
*/

#include "vga.h"

VGA::VGA() {}

VGA::~VGA() {}

size_t VGA::terminal_row = 0;
size_t VGA::terminal_column = 0;
uint8_t VGA::terminal_color = 0;
uint16_t* VGA::terminal_buffer = 0;

bool VGA::isWelcome = true;
 
size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void VGA::terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void VGA::terminal_setcolor(uint8_t color) {
	terminal_color = color;
}
 
void VGA::terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void VGA::terminal_putchar(char c) {
	if (c == '\n') {
		terminal_column = 0;
		terminal_row++;
		if (terminal_row == VGA_HEIGHT)
			terminal_initialize();
		if(!isWelcome) {
			terminal_write("$ ", strlen("$ "));
			terminal_column = 2;
		}
	}
	else if (c == '\0') {}
	else if (c == '\b') {
		if (terminal_column > 2) {
			terminal_column--;
			terminal_putentryat('\0', terminal_color, terminal_column, terminal_row);
		}
		else {
			terminal_column = 2;
		}
	}
	else {
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		if (++terminal_column == VGA_WIDTH) {
			terminal_column = 0;
			if (++terminal_row == VGA_HEIGHT)
				terminal_initialize();
		}
	}	
}
 
void VGA::terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}
 
