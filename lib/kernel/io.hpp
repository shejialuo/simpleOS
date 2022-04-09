#ifndef _LIB_KERNEL_IO_HPP
#define _LIB_KERNEL_IO_HPP

#include "stdint.hpp"

/*
  * This file is used to encapsulate the
  * assembly code to do I/O functionality
*/

/*
  * A function to wrap assembly code to write
  * a byte to port

  * param:
    1. the port need to write
    2. the data written to the port
  * return: void
*/
static inline void outb(uint16_t port, uint8_t data) {
  asm volatile ("outb %b0, %w1" : : "a" (data), "Nd" (port));
}

/*
  * A function to write the some number of word from
  * a specified address to port

  * param:
    1. port need to write
    2. the address written from
    3. the number of word
  * return: void
*/
static inline void outsw(uint16_t port, const void* address, uint32_t wordCount) {
  asm volatile ("cld; rep outsw" : "+S" (address), "+c" (wordCount) : "d" (port));
}

/*
  * A function to read data from port

  * param: port need to read
  * return: a btye data
*/
static inline uint8_t inb(uint16_t port) {
  uint8_t data;
  asm volatile ("inb %w1, %b0" : "=a" (data) : "Nd" (port));
  return data;
}

/*
  * A functio to read data from a port and
  * write some number of word to a specified address
*/
static inline void insw(uint16_t port, void* address, uint32_t wordCount) {
  asm volatile ("cld; rep insw" : "+D" (address), "+c" (wordCount) : "d" (port) : "memory");
}

#endif // _LIB_KERNEL_IO_HPP
