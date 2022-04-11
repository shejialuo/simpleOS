#ifndef _KERNEL_MEMORY_T_HPP
#define _KERNEL_MEMORY_T_HPP

#include "stdint.hpp"
#include "bitmap.hpp"

constexpr int32_t PG_SIZE = 4096;

/*
  * Due to the reason that PCB will cost 1 page size memory
  * and the start address should be `0xXXXXX000`, the stop
  * address should be `0xXXXXXfff`.
  *
  * So we make `0xc009f000` is the kernel process stack top
  * so the PCB of the kernel process shoudl be `0xc009e000`.
  * So we make bitmap address be `0xc009a000` to store 128M
  * memory.
*/
constexpr uint32_t MEM_BITMAP_BASE = 0xc009a000;
constexpr uint32_t K_HEAP_START = 0xc0100000;

constexpr uint8_t PG_P_1 = 1;  // persistent
constexpr uint8_t PG_P_0 = 0;  // not persistent
constexpr uint8_t PG_RW_R = 0; // read permission
constexpr uint8_t PG_RW_W = 2; // read and write permission
constexpr uint8_t PG_US_S = 0; // system level
constexpr uint8_t PG_US_U = 4; // user level

enum class PoolFlag {
  KERNEL = 1,
  USER = 2,
};

struct VirtualAddress {
  bitmap_t virtualBitmap;
  uint32_t virtualAddressStart;
};

struct pool {
  bitmap_t poolBitmap;
  uint32_t physicalAddressStart;
  uint32_t poolSize;
};

using vaddress_t = struct VirtualAddress;
using pool_t = struct pool;

#endif // _KERNEL_MEMORY_T_HPP
