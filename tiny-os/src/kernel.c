#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "interrupts.h"
#include "stdlib/headers/terminal.h"
#include "stdlib/headers/stdio.h"

__attribute__ ((interrupt)) void isr0(struct iframe* frame) {
	terminal_writestring_color("How do you do, fellow kids?\n", vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK));
	(void)frame;
}

void kernel_main(void) {
    init_idt();

	terminal_initialize();

    terminal_printf("Printf str test: %s\nPrintf int test: %d\nPrintf negative int test: %d\nPrintf char test: %c\n\n", "testing string", 12345678, -123456, '@');
    terminal_writestring_color("(c) left copied", vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));

    asm volatile ("sti");
    asm volatile ("int $0x80");
}
