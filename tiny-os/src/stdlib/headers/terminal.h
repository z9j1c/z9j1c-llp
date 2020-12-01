#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "string.h"

#define BYTES_PER_TERMINAL_SYM 2

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

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

extern size_t terminal_row;
extern size_t terminal_column;
extern uint16_t* terminal_buffer;

void terminal_erase_line( int line_index );

void terminal_initialize(void);

void terminal_scroll_line_up();

void terminal_putentryat( char c, uint8_t color, size_t x, size_t y );

void terminal_putchar_color( char c, uint8_t color );

void terminal_write( const char* data, size_t size, uint8_t color );

void terminal_writestring( const char* data );

void terminal_writestring_color( const char* data, uint8_t color );