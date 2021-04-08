#pragma once

#include <stddef.h>
#include <stdint.h>

size_t strlen( const char* str );

void* memcpy( void* dest, const void* src, size_t num );

void* memset ( void * ptr, int value, size_t num );

int memcmp( const void* str1, const void* str2, size_t count );