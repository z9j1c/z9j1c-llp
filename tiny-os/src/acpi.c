#include "acpi.h"
#include <stddef.h>
#include <stdint.h>

static struct acpi_rsdp* find_rsdp_in_region(void* start, size_t len) {
    for (size_t i = 0; i < len - 8; i++) {
        void* addr = (uint8_t*)start + i;
        if (memcmp("RSD PTR ", (const char*)addr, 8) == 0) {
            return (struct acpi_rsdp*)addr;
        }
    }
    return NULL;
}

struct acpi_sdt* acpi_find_rsdt() {
    // 1KB of EBDA.
    void* ebda_addr = (void*)((*(uint16_t*)0x40e) << 4);
    struct acpi_rsdp* rsdp = find_rsdp_in_region(ebda_addr, 1024);
    if (!rsdp) {
        // Static memory region.
        rsdp = find_rsdp_in_region((void*)0xe0000, 0xfffff - 0xe0000);
    }

    if (!rsdp) {
        return NULL;
    }

    uint8_t rsdp_checksum = rsdp_validate(rsdp);
    if (rsdp->revision) {
        // Zero --> ver. 1.0
        panic("UNSUPPORTED RSDP VERSION");
    }
    if (rsdp_checksum) {
        panic("INVALID RSDP CHECKSUM: %x", rsdp_checksum);
    }

    return (struct acpi_sdt*)rsdp->rsdt_addr;
}

struct acpi_sdt* acpi_find_sdt(struct acpi_sdt* root, const char* signature) {
    size_t sz = (root->header.length - sizeof(root->header)) / 4;
    for (size_t i = 0; i < sz; i++) {
        if (memcmp(signature, &root->entries[i]->header.signature, 4) == 0) {
            return root->entries[i];
        }
    }
    return NULL;
}

uint8_t rsdp_validate(struct acpi_rsdp* rstd) {
    uint32_t checksum = 0;
    for (size_t byte_index = 0; byte_index < sizeof(struct acpi_rsdp); ++byte_index) {
        checksum += ((uint8_t*)rstd)[byte_index];
    }

    return checksum & 0xFF;
}

struct acpi_sdt* acpi_find_and_validate_rsdt() {
    struct acpi_std* rsdt = acpi_find_rsdt();
    
    if (rsdt != NULL) {
        uint16_t rsdt_checksum = validate_rsdt(rsdt);
        if (rsdt_checksum) {
            panic("INVALID RSDT CHECKSUM: %x", rsdt_checksum);
        }
    }
    
    return rsdt;
}

uint16_t validate_rsdt(struct acpi_sdt* rsdt) {
    uint16_t checksum = 0;
    for (size_t byte_index = 0; byte_index < sizeof(struct acpi_sdt); ++byte_index) {
        checksum += ((uint8_t*)rsdt)[byte_index];
        checksum &= 0x100;
    }

    return checksum;
}