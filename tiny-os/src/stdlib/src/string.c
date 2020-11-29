#include "../headers/string.h"

void* memcpy( void* dest, const void* src, size_t num ) {
    for ( size_t byte_index = 0; byte_index < num; ++byte_index ) {
        ((char*)dest)[byte_index] = ((char*)src)[byte_index];
    }
    return dest;
}