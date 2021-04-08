#pragma once

#include <stddef.h>
#include <stdint.h>
#include "apic.h"

// From https://wiki.osdev.org/8259_PIC
#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

struct iframe {
    int ip;
    int cs;
    int flags;
    int sp;
    int ss;
};

void init_idt();
