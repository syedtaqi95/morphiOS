/*
    vga.h - VGA class header to communicate in text or graphics mode

    References:
    - https://files.osdev.org/mirrors/geezer/osd/graphics/modes.c
*/

#ifndef VGA_H
#define VGA_H

#include "common/types.h"
#include "common/common.h"
#include "kernel/port.h"

namespace morphios {
namespace drivers {

// Number of registers
#define	VGA_NUM_SEQ_REGS	5
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS		9
#define	VGA_NUM_AC_REGS		21

class VGA {
protected:

    // VGA Ports
    kernel::Port8Bit VGA_AC_INDEX;
    kernel::Port8Bit VGA_AC_WRITE;
    kernel::Port8Bit VGA_AC_READ;
    kernel::Port8Bit VGA_MISC_READ;
    kernel::Port8Bit VGA_MISC_WRITE;
    kernel::Port8Bit VGA_SEQ_INDEX;
    kernel::Port8Bit VGA_SEQ_DATA;
    kernel::Port8Bit VGA_GC_INDEX;
    kernel::Port8Bit VGA_GC_DATA;
    kernel::Port8Bit VGA_CRTC_INDEX;
    kernel::Port8Bit VGA_CRTC_DATA;
    kernel::Port8Bit VGA_INSTAT_READ;

    // Graphics mode resolution settings
    common::uint32_t VGA_GRAPHICS_MODE_WIDTH;
    common::uint32_t VGA_GRAPHICS_MODE_HEIGHT;
    common::uint32_t VGA_GRAPHICS_MODE_COLOURDEPTH;
    common::uint8_t *frameBufferSegment;

    /* VGA colour constants. */
    // TODO: increase this to all 256 colours
    enum vga_colour {
        VGA_COLOUR_BLACK = 0,
        VGA_COLOUR_BLUE = 1,
        VGA_COLOUR_GREEN = 2,
        VGA_COLOUR_CYAN = 3,
        VGA_COLOUR_RED = 4,
        VGA_COLOUR_MAGENTA = 5,
        VGA_COLOUR_BROWN = 6,
        VGA_COLOUR_LIGHT_GREY = 7,
        VGA_COLOUR_DARK_GREY = 8,
        VGA_COLOUR_LIGHT_BLUE = 9,
        VGA_COLOUR_LIGHT_GREEN = 10,
        VGA_COLOUR_LIGHT_CYAN = 11,
        VGA_COLOUR_LIGHT_RED = 12,
        VGA_COLOUR_LIGHT_MAGENTA = 13,
        VGA_COLOUR_LIGHT_BROWN = 14,
        VGA_COLOUR_WHITE = 15,
    };

    // Graphics mode methods
    void writeRegisters(common::uint8_t* registers);
    common::uint8_t* getFrameBufferSegment();
    virtual common::uint8_t getColorIndex(common::uint8_t r, common::uint8_t g, common::uint8_t b);

    // Text mode methods
    static inline common::uint8_t vga_entry_colour(enum vga_colour fg, enum vga_colour bg) {
        return fg | bg << 4;
    }
    
    static inline common::uint16_t vga_entry(unsigned char uc, common::uint8_t colour) {
        return (common::uint16_t) uc | (common::uint16_t) colour << 8;
    }

    void terminal_setcolour(common::uint8_t colour);
    void terminal_putentryat(char c, common::uint8_t colour, common::size_t x, common::size_t y);
    void terminal_putchar(char c);

public:
    // Text mode variables
    // Defined as static as they are global in scope
    static const common::size_t VGA_TEXT_MODE_WIDTH = 80;
    static const common::size_t VGA_TEXT_MODE_HEIGHT = 25;
    static common::size_t terminal_row;
    static common::size_t terminal_column;
    static common::uint8_t terminal_colour;
    static common::uint16_t *terminal_buffer;
    static bool isWelcome; // For checking if it is a welcome message, stops printing "$ "  

    // Public methods
    VGA(); // Constructor
    ~VGA(); // Destructor

    // Text mode methods
    void terminal_initialize(void);
    void terminal_write(const char* data, common::size_t size);
    void print_welcome_msg();

    // Graphics mode methods
    virtual bool setMode(common::uint32_t width, common::uint32_t height, common::uint32_t colourDepth);
    virtual bool supportsMode(common::uint32_t width, common::uint32_t height, common::uint32_t colourDepth);    
    virtual void putPixel(common::int32_t x, common::int32_t y, common::uint8_t r, common::uint8_t g, common::uint8_t b);
    virtual void putPixel(common::int32_t x, common::int32_t y, common::uint8_t colourIndex);
    virtual void putLine(common::int32_t x0, common::int32_t y0, common::int32_t x1, common::int32_t y1,
        common::uint8_t r, common::uint8_t g, common::uint8_t b);
    virtual void putRect(common::int32_t x0, common::int32_t y0, common::int32_t w, common::int32_t h, 
        common::uint8_t r, common::uint8_t g, common::uint8_t b);
};

} // namespace drivers    
} // namespace morphios

#endif // VGA_H