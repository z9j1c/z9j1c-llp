#include "spinlock.h"

int global_kernel_lock = 0;

inline void spin_lock(int *p) {
    while(!__sync_bool_compare_and_swap(p, 0, 1));
}

inline void spin_unlock(int volatile *p) {
    asm volatile ("":::"memory");
    *p = 0;
}

void kernel_lock() {
    spin_lock(&global_kernel_lock);
}

void kernel_unlock() {
    spin_unlock(&global_kernel_lock);
}