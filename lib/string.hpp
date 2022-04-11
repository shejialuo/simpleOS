#ifndef _LIB_STRING_HPP
#define _LIB_STRING_HPP

#include "stdint.hpp"
void memset(void* startAddress_, uint8_t value, uint32_t size);
void memcpy(void* destination_, void* source_, uint32_t size);
int8_t memcmp(void* memoryA_, void* memoryB_, uint32_t size);
char* strcpy(char* destination_, const char* source_);
uint32_t strlen(const char* str);
int8_t strcmp(const char* stringA, const char* stringB);
char* strchr(const char* string, const uint8_t character);
char* strrchr(const char* string, const uint8_t character);
char* strcat(char* destination_, const char* source_);
uint32_t strchrs(const char* string, const uint8_t character);

#endif // _LIB_STRING_HPP
