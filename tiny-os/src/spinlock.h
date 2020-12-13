#pragma once

inline void spin_lock(int *p);

inline void spin_unlock(int volatile *p);

void kernel_lock();

void kernel_unlock();