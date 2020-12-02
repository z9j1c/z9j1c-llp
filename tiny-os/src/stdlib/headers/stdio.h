#pragma once

#include <stddef.h>
#include <stdarg.h>

#include "string.h"
#include "terminal.h"

void middle_buff_terminal_print( const char* partial_str, size_t len, int force_print );

int va_terminal_printf( const char* format, va_list args );

int terminal_printf( const char * format, ... );