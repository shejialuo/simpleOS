#ifndef _LIB_KERNEL_PRINT_HPP
#define _LIB_KERNEL_PRINT_HPP

#include "stdint.hpp"

// disable c++ mangling
extern "C" void put_char(uint8_t);
extern "C" void put_str(char*);
extern "C" void put_int(uint32_t);
extern "C" void set_cursor(uint32_t);

void put_char(uint8_t char_asci);
void put_str(char* message);
void put_int(uint32_t num);
void set_cursor(uint32_t cursorPosition);

#endif // _LIB_KERNEL_PRINT_HPP
