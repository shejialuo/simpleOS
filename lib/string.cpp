#include "global.hpp"
#include "debug.hpp"
#include "string.hpp"

/*
  * A function to mirror the functionality of C `memset`

  * param:
    1. The start address written to
    2. The data written to
    3. The byte size
  * return: void
*/
void memset(void* startAddress_, uint8_t value, uint32_t size) {
  assert(startAddress_ != nullptr);
  uint8_t* startAddress = static_cast<uint8_t*>(startAddress_);
  while(size-- > 0) {
    *startAddress++ = value;
  }
}

/*
  * A function to mirror the functionality of C `memcpy`

  * param:
    1. The destination
    2. The source
    3. The size
  * return: void
*/
void memcpy(void* destination_, void* source_, uint32_t size) {
  assert(destination_ != nullptr && source_!= nullptr);
  uint8_t* destination = static_cast<uint8_t*>(destination_);
  const uint8_t* source = const_cast<uint8_t*>(static_cast<uint8_t*>(source_));
  while(size-- > 0) {
    *destination++ = *source++;
  }
}

/*
  * A function to mirror the functionality of C `memcmp`

  * param:
    1. The memory address A
    2. The memory address B
    3. The size
  * return:
    + 1  when memoryA > memoryB
    + -1 when memoryA < memoryB
    + 0  when memoryA = memoryB
*/
int8_t memcmp(void* memoryA_, void* memoryB_, uint32_t size) {
  assert(memoryA_ != nullptr && memoryB_!= nullptr);
  const uint8_t* memoryA = const_cast<uint8_t*>(static_cast<uint8_t*>(memoryA_));
  const uint8_t* memoryB = const_cast<uint8_t*>(static_cast<uint8_t*>(memoryB_));
  while(size-- > 0) {
    if(*memoryA != * memoryB) {
      return *memoryA > *memoryB ? 1 : -1;
    }
    memoryA++;
    memoryB++;
  }
  return 0;
}

/*
  * A function to mirror the functionality of C `strcpy`

  * param:
    1. The destination
    2. The source
  * return: the destination source
*/
char* strcpy(char* destination_, const char* source_) {
  assert(destination_ != nullptr && source_!= nullptr);
  char *destination = destination_;
  while(*destination_++ = *source_++) ;
  return destination;
}

/*
  * A function to return the length of string

  * param: the string
  * return: the length of the string
*/
uint32_t strlen(const char* str) {
  assert(str != nullptr);
  const char* p = str;
  while(*p++);
  return (p - str - 1);
}

/*
  * A function to compare the string A and the string B

  * param:
    1. The string A
    2. The string B
  * return:
    + 1  when string A > string B
    + -1 when string A < string B
    + 0  when string A = string B
*/
int8_t strcmp(const char* stringA, const char* stringB) {
  assert(stringA != nullptr && stringB != nullptr);
  while(*stringA != 0 && *stringA == *stringB) {
    stringA++;
    stringB++;
  }
  return *stringA < *stringB ? -1 : *stringA > *stringB;
}

/*
  * A function to mirror the functionality of `strchr`

  * param:
    1. the string
    2. the character want to find
  * return: the character address
*/
char* strchr(const char* string, const uint8_t character) {
  assert(string != nullptr);
  while(*string != 0) {
    if (*string == character) {
      return const_cast<char*>(string);
    }
    string++;
  }
  return nullptr;
}

/*
  * A function to mirror the functionality of `strrchr`

  * param:
    1. the string
    2. the character want to find
  * return: the character address
*/
char* strrchr(const char* string, const uint8_t character) {
  assert(string != nullptr);
  const char* lastChar = nullptr;
  while(*string != 0) {
    if (*string == character) {
      lastChar = string;
    }
    string++;
  }
  return const_cast<char*>(lastChar);
}

/*
  * A function to mirror the functionality of `strcat`

  * param:
    1. the string source
    2. the string destination
  * return: the destination address
*/
char* strcat(char* destination_, const char* source_) {
  assert(destination_ != nullptr && source_ != nullptr);
  char* string = destination_;
  while(*string++);
  --string;
  while((*string++ = *source_++));
  return destination_;
}

/*
  * A function to mirror the functionality of `strchrs`

  * param:
    1. the string
    2. the character
  * return: the number of character in the string
*/
uint32_t strchrs(const char* string, const uint8_t character) {
  assert(string != nullptr);
  uint32_t characterCount = 0;

  while(*string != 0) {
    if(*string == character) {
      characterCount++;
    }
    string++;
  }

  return characterCount;
}
