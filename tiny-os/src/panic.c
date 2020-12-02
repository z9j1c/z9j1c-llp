#include "panic.h"

static int panic_state_flag = 0;

struct PANIC_REFS_SET {
    intptr_t eax;
    intptr_t ebx;
    intptr_t ecx;
    intptr_t edx;
    u_int16_t ds;
    intptr_t esi;
    u_int16_t es;
    intptr_t edi;
    u_int16_t ss;
    intptr_t esp;
    intptr_t ebp;
    u_int16_t cs;
} panic_registers;

void disable_irq() {
    asm volatile ("cli");
}


void panic( const char* format, ... ) {
    // TODO: add two power format into printf and print registers
    asm("movl %%eax,%0" : "=r"(panic_registers.eax));
    asm("movl %%ebx,%0" : "=r"(panic_registers.ebx));
    asm("movl %%ecx,%0" : "=r"(panic_registers.ecx));
    asm("movl %%edx,%0" : "=r"(panic_registers.edx));
    asm("movl %%esi,%0" : "=r"(panic_registers.esi));
    asm("movl %%edi,%0" : "=r"(panic_registers.edi));
    asm("movl %%esp,%0" : "=r"(panic_registers.esp));
    asm("movl %%ebp,%0" : "=r"(panic_registers.ebp));
    asm("movw %%ds,%0" : "=r"(panic_registers.ds));
    asm("movw %%es,%0" : "=r"(panic_registers.es));
    asm("movw %%ss,%0" : "=r"(panic_registers.ss));
    asm("movw %%cs,%0" : "=r"(panic_registers.cs));

    
    // Under the assumption: we have only one CPU
    disable_irq();
    __sync_fetch_and_add(&panic_state_flag, 1);

    // TODO: clear all system spinlocks
    // bust_spinlocks(1);
    
    // Printf uses static buffer only, so it's ok to call it
    va_list args;
    va_start(args, format);
    panic_print(format, args);
    va_end(args);

    // --> sleep --> get NMI --> repeat
    cpu_relax();
}


void nmi_panic( const char* format, ... ) {
    int old_panic_state = __sync_val_compare_and_swap(&panic_state_flag, 0, 1);

    if (old_panic_state) {
        cpu_relax();
    } else {
        va_list args;
        va_start(args, format);
        va_terminal_printf(format, args);
        va_end(args);
    }
}


void cpu_relax() {
    while (1) {
        asm volatile ("hlt");
    }
}

void panic_print( const char* format, va_list args ) {
    terminal_writestring_color("*=============*\n     PANIC\n*=============*\n", vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
    va_terminal_printf(format, args);
}