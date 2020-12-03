#pragma once

#include <stdarg.h>
#include <stdint.h>
#include <sys/types.h>
#include "stdlib/headers/terminal.h"
#include "stdlib/headers/stdio.h"

// Is necessary for dealing with NMI
static int panic_state_flag;

struct PANIC_REFS_SET;

// Panic version if we got NMI ==> just go out
// Yeah, the idea is stolen from linux src 
void nmi_panic( const char* format, ... );

// Panic: arguments as for printf
// Made like https://github.com/torvalds/linux/blob/master/kernel/panic.c
void panic( const char* format, ... );

// Print panic messages: can be called from various panic variants
void panic_print( const struct PANIC_REFS_SET* regs, const char* format, va_list args );

// Disable all interrupts except NMI
void disable_irq();

// Infinite loop with minimal power usage
void cpu_relax();