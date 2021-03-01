/*
    vga.cpp - VGA class implementation to communicate with stdout

	References:
    - https://files.osdev.org/mirrors/geezer/osd/graphics/modes.c
*/

#include "drivers/vga.h"

using namespace morphios::common;
using namespace morphios::drivers;
using namespace morphios::kernel;

VGA::VGA()
  : VGA_AC_INDEX(0x3C0),
	VGA_AC_WRITE(0x3C0),
	VGA_AC_READ(0x3C1),
	VGA_MISC_READ(0x3CC),
	VGA_MISC_WRITE(0x3C2),
	VGA_SEQ_INDEX(0x3C4),
	VGA_SEQ_DATA(0x3C5),
	VGA_GC_INDEX(0x3CE),
	VGA_GC_DATA(0x3CF),
	VGA_CRTC_INDEX(0x3D4),
	VGA_CRTC_DATA(0x3D5),
	VGA_INSTAT_READ(0x3DA)
{}

VGA::~VGA() {}

// Text mode methods

size_t VGA::terminal_row = 0;
size_t VGA::terminal_column = 0;
uint8_t VGA::terminal_colour = 0;
uint16_t* VGA::terminal_buffer = 0;
bool VGA::isWelcome = true;

void VGA::terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_colour = vga_entry_colour(VGA_COLOUR_GREEN, VGA_COLOUR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_TEXT_MODE_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_TEXT_MODE_WIDTH; x++) {
			const size_t index = y * VGA_TEXT_MODE_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_colour);
		}
	}
}

void VGA::terminal_setcolour(uint8_t colour) {
	terminal_colour = colour;
}
 
void VGA::terminal_putentryat(char c, uint8_t colour, size_t x, size_t y) {
	const size_t index = y * VGA_TEXT_MODE_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, colour);
}

void VGA::terminal_putchar(char c) {
	if (c == '\n') {
		terminal_column = 0;
		terminal_row++;
		if (terminal_row == VGA_TEXT_MODE_HEIGHT)
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
			terminal_putentryat('\0', terminal_colour, terminal_column, terminal_row);
		}
		else {
			terminal_column = 2;
		}
	}
	else {
		terminal_putentryat(c, terminal_colour, terminal_column, terminal_row);
		if (++terminal_column == VGA_TEXT_MODE_WIDTH) {
			terminal_column = 0;
			if (++terminal_row == VGA_TEXT_MODE_HEIGHT)
				terminal_initialize();
		}
	}	
}
 
void VGA::terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void VGA::print_welcome_msg() {
	kprintf("                               __    _ ____  _____\n");
	kprintf("   ____ ___  ____  _________  / /_  (_) __ \\/ ___/\n");
	kprintf("  / __ `__ \\/ __ \\/ ___/ __ \\/ __ \\/ / / / /\\__ \\ \n");
	kprintf(" / / / / / / /_/ / /  / /_/ / / / / / /_/ /___/ / \n");
	kprintf("/_/ /_/ /_/\\____/_/  / .___/_/ /_/_/\\____//____/  \n");
	kprintf("                    /_/                           \n");
	kprintf("\nWake up, Neo\n");
	kprintf("The Matrix has you\nFollow the white rabbit\n...\nKnock, Knock, Neo.\n");	
}

// Graphics mode methods

bool VGA::setMode(common::uint32_t width, common::uint32_t height, common::uint32_t colourDepth) {
	if (!supportsMode(width, height, colourDepth))
		return false;
	
	this->VGA_GRAPHICS_MODE_WIDTH = width;
	this->VGA_GRAPHICS_MODE_HEIGHT = height;
	this->VGA_GRAPHICS_MODE_COLOURDEPTH = colourDepth;
	
	// register sets
	unsigned char g_320x200x256[] =	{
	/* MISC */
		0x63,
	/* SEQ */
		0x03, 0x01, 0x0F, 0x00, 0x0E,
	/* CRTC */
		0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
		0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
		0xFF,
	/* GC */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
		0xFF,
	/* AC */
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
		0x41, 0x00, 0x0F, 0x00,	0x00
	};

	writeRegisters(g_320x200x256);
	return true;
}

// TODO: add support for other display modes
bool VGA::supportsMode(common::uint32_t width, common::uint32_t height, common::uint32_t colourDepth) {
	if ((width == 320) && (height == 200) && (colourDepth == 8))
		return true;
	return false;
}

void VGA::writeRegisters(common::uint8_t* regs) {

	// Write Misc registers
	VGA_MISC_WRITE.write(*(regs++));

	// Write Sequencer registers
	for (uint8_t i = 0; i < VGA_NUM_SEQ_REGS; i++) {
		VGA_SEQ_INDEX.write(i);
		VGA_SEQ_DATA.write(*(regs++));
	}

	// Unlock CRTC registers
	VGA_CRTC_INDEX.write(0x03);
	VGA_CRTC_DATA.write(VGA_CRTC_DATA.read() | 0x80);
	VGA_CRTC_INDEX.write(0x11);
	VGA_CRTC_DATA.write(VGA_CRTC_DATA.read() & ~0x80);

	// Make sure they remain unlocked
	regs[0x03] |= 0x80;
	regs[0x11] &= ~0x80;

	// Write CRTC registers
	for(uint8_t i = 0; i < VGA_NUM_CRTC_REGS; i++) {
		VGA_CRTC_INDEX.write(i);
		VGA_CRTC_DATA.write(*(regs++));
	}

	// Write Graphics Controller registers
	for(uint8_t i = 0; i < VGA_NUM_GC_REGS; i++) {
		VGA_GC_INDEX.write(i);
		VGA_GC_DATA.write(*(regs++));
	}

	// Write Attribute Controller registers
	for(uint8_t i = 0; i < VGA_NUM_AC_REGS; i++) {
		VGA_INSTAT_READ.read();
		VGA_AC_INDEX.write(i);
		VGA_AC_WRITE.write(*(regs++));
	}

	// Lock 16-colour palette and unblank display
	VGA_INSTAT_READ.read();
	VGA_AC_INDEX.write(0x20);
}


void VGA::putPixel(common::uint32_t x, common::uint32_t y,  common::uint8_t r, common::uint8_t g, common::uint8_t b) {
	putPixel(x, y, getColorIndex(r, g, b));
}

void VGA::putPixel(common::uint32_t x, common::uint32_t y, common::uint8_t colourIndex) {
	uint8_t *pixelAddr = getFrameBufferSegment() + VGA_GRAPHICS_MODE_WIDTH*y + x;
	*pixelAddr = colourIndex;
}

// Converts RGB colours to the VGA colour
// TODO: add all 256 VGA colours
uint8_t VGA::getColorIndex(common::uint8_t r, common::uint8_t g, common::uint8_t b) {
	if(r == 0x00, g == 0x00, b == 0xA8) // blue
        return VGA_COLOUR_BLUE;
    return 0x00;
}

// VGA framebuffer is at A000:0000, B000:0000, or B800:0000
// depending on bits in GC 6
uint8_t* VGA::getFrameBufferSegment() {
	VGA_GC_INDEX.write(0x06);
	uint8_t segment = VGA_GC_DATA.read() & (3<<2);
	switch(segment) {
        default:
        case 0<<2: return (uint8_t*)0x00000;
        case 1<<2: return (uint8_t*)0xA0000;
        case 2<<2: return (uint8_t*)0xB0000;
        case 3<<2: return (uint8_t*)0xB8000;
    }
}

