#pragma once

#include <stddef.h>
#include <stdarg.h>

#include "string.h"
#include "terminal.h"

// Terminal buffering print
void middle_buff_terminal_print( const char* partial_str, size_t len, int force_print );

int va_terminal_printf( const char* format, va_list args );

int terminal_printf( const char * format, ... );

// Pass number representation into middle_buff_terminal_print()
// Max base is 16
static int terminal_num_print( int value, int base );