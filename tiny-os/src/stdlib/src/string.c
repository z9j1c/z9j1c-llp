#include "../headers/string.h"

size_t strlen( const char* str ) {
	size_t len = 0;
	while (str[len]) {
		++len;
    }
	return len;
}

void* memcpy( void* dest, const void* src, size_t num ) {
    // Case: -----> [src-start --> (dest-start --> src-end] --> dest-end) ----->
    if ( src + num > dest ) {
        for ( size_t byte_index = num; byte_index > 0; --byte_index ) {
            ((char*)dest)[byte_index - 1] = ((char*)src)[byte_index - 1];
        }
    } else {
        for ( size_t byte_index = 0; byte_index < num; ++byte_index ) {
            ((char*)dest)[byte_index] = ((char*)src)[byte_index];
        }
    }

    return dest;
}

void* memset ( void* ptr, int value, size_t num ) {
    for (size_t byte_index = 0; byte_index < num; ++byte_index) {
        ((char*)ptr)[byte_index] = value;
    }
    return ptr;
}

int memcmp( const void* str1, const void* str2, size_t count ) {
    const unsigned char *s1 = (const unsigned char*)str1;
    const unsigned char *s2 = (const unsigned char*)str2;

    while (count-- > 0) {
        if (*s1++ != *s2++) {
            return s1[-1] < s2[-1] ? -1 : 1;
        }
    }
    return 0;
}