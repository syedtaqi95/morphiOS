/*
    vga.h - VGA class header to communicate with stdout
*/

#ifndef VGA_H
#define VGA_H

#include "common/types.h"
#include "common/common.h"

class VGA {
private:
    // Private members
    /* Hardware text mode color constants. */
    enum vga_color {
        VGA_COLOR_BLACK = 0,
        VGA_COLOR_BLUE = 1,
        VGA_COLOR_GREEN = 2,
        VGA_COLOR_CYAN = 3,
        VGA_COLOR_RED = 4,
        VGA_COLOR_MAGENTA = 5,
        VGA_COLOR_BROWN = 6,
        VGA_COLOR_LIGHT_GREY = 7,
        VGA_COLOR_DARK_GREY = 8,
        VGA_COLOR_LIGHT_BLUE = 9,
        VGA_COLOR_LIGHT_GREEN = 10,
        VGA_COLOR_LIGHT_CYAN = 11,
        VGA_COLOR_LIGHT_RED = 12,
        VGA_COLOR_LIGHT_MAGENTA = 13,
        VGA_COLOR_LIGHT_BROWN = 14,
        VGA_COLOR_WHITE = 15,
    };

    // Private methods
    static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
        return fg | bg << 4;
    }
    
    static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
        return (uint16_t) uc | (uint16_t) color << 8;
    }

    void terminal_setcolor(uint8_t color);
    void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
    void terminal_putchar(char c);

public:
    // Public variables
    // Defined as static as they are global in scope
    static const size_t VGA_WIDTH = 80;
    static const size_t VGA_HEIGHT = 25;
    static size_t terminal_row;
    static size_t terminal_column;
    static uint8_t terminal_color;
    static uint16_t *terminal_buffer;
    static bool isWelcome; // For checking if it is a welcome message, stops printing "$ "

    // Public methods
    VGA(); // Constructor
    ~VGA(); // Destructor
    void terminal_initialize(void);
    void terminal_write(const char* data, size_t size);
};



#endif // VGA_H