#include "idt.h"
#include "ps2_keyboard.h"

struct idt_entry {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t zero;
    uint8_t flags;
    uint16_t base_hi;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct idt_entry IDT[256];
struct idt_ptr IDTPTR;

static void idt_register(uint8_t pos, uint32_t base, uint16_t sel, uint8_t flags) {
    IDT[pos].base_lo = base & 0xFFFF;
    IDT[pos].base_hi = (base >> 16) & 0xFFFF;

    IDT[pos].sel = sel;
    IDT[pos].zero = 0;
    IDT[pos].flags = flags;
}

extern void timer_isr();
extern void keyboard_isr();
extern void syscall_entry();

__attribute__ ((interrupt)) void dummy_isr(struct iframe* frame) {
    (void)frame;
}

__attribute__ ((interrupt)) void spurious_isr(struct iframe* frame) {
    (void)frame;
    apic_eoi();
}

// Idea from https://wiki.osdev.org/8259_PIC
static void PIC_remap() {
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);
    outb(PIC1_DATA, 0x20);
    outb(PIC2_DATA, 40);
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);
    outb(PIC1_DATA, 0x0);
    outb(PIC2_DATA, 0x0);
}

void init_idt() {
    IDTPTR.limit = sizeof(IDT) - 1;
    IDTPTR.base = (uint32_t)&IDT;

    PIC_remap();

    // For flags param see https://wiki.osdev.org/IDT#Structure_IA-32.
    for (size_t i = 0; i < 256; i++) {
        idt_register(i, (uint32_t)dummy_isr, 0x08, 0b10001110);
    }

    idt_register(128, (uint32_t)syscall_entry, 0x08, 0b11101110);
    idt_register(32, (uint32_t)timer_isr, 0x08, 0b10001110);
    idt_register(40, (uint32_t)keyboard_isr, 0x08, 0b10001110);
    idt_register(39, (uint32_t)spurious_isr, 0x08, 0b10001110);

    asm volatile (
        "lidt (%0)\n"
        :
        : "r"(&IDTPTR)
        :
    );
}
