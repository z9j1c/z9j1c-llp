#include "../headers/stdio.h"

enum vga_color stdio_current_color;

void middle_buff_terminal_print( const char* partial_str, size_t len, int force_print ) {
    enum {middle_size = 128};
    static char middle_buf[middle_size]; // Cumulative buffer for batch printing
    static size_t middle_buf_index = 0;

    if ( middle_buf_index + len >= middle_size ) {
        /*
            If input string is too long:
            1. put as much as possible into middle buffer
            2. flush it
            3. retry with string suffix
        */
        int passed_part_size = middle_size - middle_buf_index;

        memcpy(middle_buf + middle_buf_index, partial_str, passed_part_size);
        terminal_write(middle_buf, middle_size, VGA_COLOR_WHITE);
        middle_buf_index = 0;

        middle_buff_terminal_print(partial_str + passed_part_size, len - passed_part_size, force_print);
    } else {
        memcpy(middle_buf + middle_buf_index, partial_str, len);
        middle_buf_index += len;
    }

    // "Flush"
    if ( force_print ) {
        terminal_write(middle_buf, middle_buf_index, VGA_COLOR_WHITE);
        middle_buf_index = 0;
    }
}

int terminal_printf( const char* format, ... ) {   
    va_list args;

    va_start(args, format);
    int symbols_printed_cnt = va_terminal_printf(format, args);
    va_end(args);

    return symbols_printed_cnt;
}

int va_terminal_printf( const char* format, va_list args ) {
    size_t format_len = strlen(format);
    size_t symbols_printed_cnt = 0;

    // Variables needed for specifiers dealing
    // [-------------]
    int decimal_arg = 0;
    char* str_ptr_arg = NULL;
    char char_arg = '\0';
    
    int decimal_arg_delimiter = 10;
    int decimal_arg_copy = 0;
    int str_arg_len = 0;
    // [-------------]

    for (size_t sym_index = 0; sym_index < format_len; ++sym_index) {
        if ( format[sym_index] == '%' ) {
            
            // Garbage slash at the end
            if (sym_index == format_len - 1) {
                break;
            }

            switch (format[sym_index + 1]) {
                // Decimal
                case 'd':
                    decimal_arg = va_arg(args, int);
                    
                    if (decimal_arg < 0){
                        middle_buff_terminal_print("-", 1, 0);
                        decimal_arg *= -1;
                        ++symbols_printed_cnt;
                    }

                    symbols_printed_cnt += terminal_num_print(decimal_arg, 10);
                    break;

                // Char
                case 'c':
                    char_arg = va_arg(args, int);
                    middle_buff_terminal_print(&char_arg, 1, 0);
                    ++symbols_printed_cnt;
                    break;

                // String
                case 's':
                    str_ptr_arg = va_arg(args, char*);
                    str_arg_len = strlen(str_ptr_arg);

                    middle_buff_terminal_print(str_ptr_arg, str_arg_len, 0);
                    symbols_printed_cnt += str_arg_len;
                    break;

                case 'x':
                    symbols_printed_cnt += 2;
                    middle_buff_terminal_print("0x", 2, 0);
                    decimal_arg = va_arg(args, int);
                    symbols_printed_cnt += terminal_num_print(decimal_arg, 16);
                    break;

                case 'o':
                    decimal_arg = va_arg(args, int);
                    symbols_printed_cnt += terminal_num_print(decimal_arg, 8);
                    break;
            }

            // We passed through double-char specifier --> additional increment
            ++sym_index;

        } else {
            // TODO: batch print
            middle_buff_terminal_print(format + sym_index, 1, 0);
            ++symbols_printed_cnt;
        }
    }

    middle_buff_terminal_print(NULL, 0, 1); // "flush"

    return symbols_printed_cnt;
}

static int terminal_num_print( int value, int base ) {
    int symbols_printed_cnt = 0;
    int delimiter = 1;
    int value_copy = value;
    char char_repr = '\0';

    while (value_copy != 0) {
        delimiter *= base;
        value_copy /= base;
        ++symbols_printed_cnt;
    }
    delimiter /= base;

    while (value != 0) {
        int digit = value / delimiter;
        
        if (digit > 9) {
            char_repr = 'A';
            digit -= 10;
        } else {
            char_repr = '0';
        }

        char_repr += digit;

        middle_buff_terminal_print(&char_repr, 1, 0);
        value %= delimiter;
        delimiter /= base;
    }

    return symbols_printed_cnt;
}