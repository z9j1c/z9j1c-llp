#pragma once
#include <stdint.h>

static inline void outb(uint16_t port, uint8_t data) {
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
}