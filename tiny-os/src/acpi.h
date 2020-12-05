#pragma once

#include <stddef.h>
#include <stdint.h>
#include "stdlib/headers/string.h"
#include "panic.h"

struct acpi_sdt_header {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemid[6];
    char oem_tableid[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} __attribute__((packed));

struct acpi_sdt {
    struct acpi_sdt_header header;

    union {
        struct acpi_sdt* entries[0];
        char data[0];
    };

} __attribute__((packed));


struct acpi_rsdp {
    char signature[8];
    uint8_t checksum;
    char oemid[6];
    uint8_t revision;
    uint32_t rsdt_addr;
} __attribute__ ((packed));

struct acpi_sdt* acpi_find_rsdt();
struct acpi_sdt* acpi_find_sdt(struct acpi_sdt* root, const char* signature);
struct acpi_sdt* acpi_find_and_validate_rsdt();

/*
    RSDP ver. 1.0 checksum
    https://wiki.osdev.org/RSDP#Checksum

    Return:
    zero - Ok
    non zero - Invalid
*/
uint8_t rsdp_validate(struct acpi_rsdp* rstp);

/*
    RSDT checksum
    https://wiki.osdev.org/RSDT#Validating_the_RSDT

    Return:
    zero - Ok
    non zero - Invalid
*/
uint16_t validate_rsdt(struct acpi_sdt* rsdt);