#pragma once

#include <stdint.h>
#include "idt.h"
#include "spinlock.h"
#include "stdlib/headers/asm.h"
#include "idt.h"

static enum {PIT_LOW_FREQ = 18}; // Frequency -- Hz
static enum {APIC_TIMER_FLAG, PIT_FLAG};

static int current_timer;

// Ticks counter, used for measuring ticks intervals
static uint64_t curr_timer_ticks_interval;

// Make CPU sleep for time interval according to PIT
void pit_sleep(uint64_t time_seconds);

// Set PIT frequency equal to 18 Hz
void set_pit_low_mode();

__attribute__ ((interrupt)) void timer_isr(struct iframe* frame);

void switch_on_apic_irq0();