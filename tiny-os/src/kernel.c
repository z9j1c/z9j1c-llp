#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "idt.h"
#include "gdt.h"
#include "acpi.h"
#include "apic.h"
#include "panic.h"
#include "memory.h"
#include "spinlock.h"
#include "stdlib/headers/string.h"
#include "stdlib/headers/terminal.h"
#include "multiboot.h"
#include "timer.h"
#include "ps2_keyboard.h"

__attribute__ ((interrupt)) void syscall_entry(struct iframe* frame) {
    terminal_writestring_color("Syscall!\n", vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
    (void)frame;
}

extern void jump_userspace();

void kernel_main(multiboot_info_t* multiboot_info, unsigned int magic) {
    init_gdt();
    init_idt();

	terminal_initialize();
    asm volatile ("sti");
    print_mapped_mem(multiboot_info);
    
    struct acpi_sdt* rsdt = acpi_find_and_validate_rsdt();
    if (!rsdt) {
        panic("RSDT not found!");
    }
    apic_init(rsdt);
}
