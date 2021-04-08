#include "../headers/terminal.h"

size_t terminal_row;
size_t terminal_column;
uint16_t* terminal_buffer;

void terminal_erase_line(int line_index) {
	uint8_t color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	const size_t line_base = line_index * VGA_WIDTH;
	
	for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = line_base + x;
			terminal_buffer[index] = vga_entry(' ', color);
		}
}

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_buffer = (uint16_t*) 0xB8000;
	
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		terminal_erase_line(y);
	}
}

void terminal_scroll_line_up() {
	memcpy((char*)terminal_buffer,
			(char*)terminal_buffer + VGA_WIDTH * BYTES_PER_TERMINAL_SYM,
			(VGA_HEIGHT - 1) * VGA_WIDTH * BYTES_PER_TERMINAL_SYM);
	terminal_erase_line(VGA_HEIGHT - 1);
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar_color(char c, uint8_t color) {
    switch (c) {
    
	case '\n':
        if (terminal_row == VGA_HEIGHT - 1) {
			terminal_scroll_line_up();
		} else {
			++terminal_row;
		}

		terminal_column = 0;
        break;

    default:
		if (terminal_column == VGA_WIDTH) {
			if (terminal_row != VGA_HEIGHT - 1) {
				++terminal_row;
			} else {
				terminal_scroll_line_up();
			}

			terminal_column = 0;
		}

        terminal_putentryat(c, color, terminal_column, terminal_row);
        ++terminal_column;
    }
}

void terminal_write(const char* data, size_t size, uint8_t color) {
	for (size_t i = 0; i < size; i++) {
        terminal_putchar_color(data[i], color);
    }
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data), vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
}

void terminal_writestring_color(const char* data, uint8_t color) {
    terminal_write(data, strlen(data), color);
}
