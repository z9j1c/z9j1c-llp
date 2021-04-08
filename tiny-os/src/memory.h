#pragma once

#include <stdint.h>
#include "multiboot.h"
#include "stdlib/headers/stdio.h"

void print_mapped_mem(multiboot_info_t* multiboot_info);