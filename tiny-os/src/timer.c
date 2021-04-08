#include "timer.h"

static int current_timer = PIT_FLAG;
static uint64_t curr_timer_ticks_interval = 0;

void pit_sleep(uint64_t time_seconds) {
    curr_timer_ticks_interval = 0;
    while (curr_timer_ticks_interval < PIT_LOW_FREQ * time_seconds) {
        asm volatile ("hlt");
    }
}

void set_pit_low_mode() {
    // https://en.wikibooks.org/wiki/X86_Assembly/Programmable_Interval_Timer
    
    asm volatile (
        "push %eax \n\t"
        "mov $0x36, %al \n\t"       // Choose channel 0
        "outb %al, $0x43 \n\t"

        "mov $0x10000, %ax \n\t"    // Set the lowest frequency - 18
        "outb %al, $0x40 \n\t"
        "mov %ah, %al \n\t"
        "outb %al, $0x40 \n\t"
        "pop %eax"
    );
}

__attribute__ ((interrupt)) void timer_isr(struct iframe* frame) {
    (void)frame;

    ++curr_timer_ticks_interval;

    switch (current_timer) {
        case APIC_TIMER_FLAG:
            apic_eoi();
            break;

        case PIT_FLAG:
            kernel_lock();
            terminal_writestring_color(".", vga_entry_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK));
            kernel_unlock();
            
            outb(PIC1_COMMAND, 0x20);
            break;
    }
}


void switch_on_apic_irq0() {
    current_timer = APIC_TIMER_FLAG;
}