#ifndef _LIB_KERNEL_BITMAP_HPP
#define _LIB_KERNEL_BITMAP_HPP

#include "global.hpp"
#include "stdint.hpp"
constexpr uint8_t BITMAP_MASK = 1;

struct Bitmap {
  uint32_t bitmapBytesLength;
  uint8_t* bits;              // the byte pointer
};

using bitmap_t = struct Bitmap;

void bitmapInitialization(bitmap_t* bitmap);
bool bitmapScanTest(bitmap_t* bitmap, uint32_t bitIndex);
int bitmapScan(bitmap_t* bitmap, uint32_t count);
void bitmapSet(bitmap_t* bitmap, uint32_t bitIndex, int8_t value);

#endif // _LIB_KERNEL_BITMAP_HPP
