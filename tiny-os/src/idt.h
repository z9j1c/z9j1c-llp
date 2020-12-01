#pragma once

#include <stddef.h>
#include <stdint.h>
#include "apic.h"


struct iframe {
    int ip;
    int cs;
    int flags;
    int sp;
    int ss;
};

void init_idt();
