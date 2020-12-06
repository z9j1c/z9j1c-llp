#include "memory.h"
#include <stdint.h>

void print_mapped_mem(multiboot_info_t* multiboot_info) {
    if (0 == (multiboot_info->flags & MULTIBOOT_INFO_MEM_MAP)) {
        terminal_printf("Can't access mapped info from multiboot");
        return;
    }

    multiboot_memory_map_t* mapped_piece = (multiboot_memory_map_t*)multiboot_info->mmap_addr;
    uint32_t multiboot_info_high_addr_bound = multiboot_info->mmap_addr + multiboot_info->mmap_length;

    terminal_printf("----- Multiboot info mmap -----\n[x] Addr | Length | TYPE\n");
    for (size_t mapped_piece_index = 0; (uint32_t)mapped_piece < multiboot_info_high_addr_bound; ++mapped_piece_index) {
        terminal_printf("[%z] %H %H ",
                        mapped_piece_index,
                        (uint32_t)mapped_piece->addr_low,
                        (uint32_t)mapped_piece->len_low);

        int mapped_type = (int)mapped_piece->type;
        switch (mapped_type) {
            case MULTIBOOT_MEMORY_AVAILABLE:
                terminal_printf("AVAILABLE\n");
                break;
            case MULTIBOOT_MEMORY_RESERVED:
                terminal_printf("RESERVED\n");
                break;
            case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
                terminal_printf("ACPI RECLAIMABLE\n");
                break;
            case MULTIBOOT_MEMORY_NVS:
                terminal_printf("NVS\n");
                break;
            case MULTIBOOT_MEMORY_BADRAM:
                terminal_printf("BADRAM\n");
                break;
        }

        mapped_piece = (multiboot_memory_map_t*)((uint32_t)mapped_piece + mapped_piece->size + sizeof(mapped_piece->size));
    }

    terminal_printf("\n");
}