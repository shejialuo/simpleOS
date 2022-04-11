#include "stdint.hpp"
#include "string.hpp"
#include "print.hpp"
#include "interrupt.hpp"
#include "debug.hpp"
#include "bitmap.hpp"

/*
  * A function to use `memset` to initialize
  * the bitmap

  * param: bitmap struct
  * return: void
*/
void bitmapInitialization(bitmap_t* bitmap) {
  memset(bitmap->bits, 0, bitmap->bitmapBytesLength);
}

/*
  * A function to test whether bitIndex is 1 or not

  * param: bitmap struct
  * return: boolean
*/
bool bitmapScanTest(bitmap_t* bitmap, uint32_t bitIndex) {
  // To get the byte index because `bitmap->bits`'s type
  // is `uint8_t`
  uint32_t byteIndex = bitIndex / 8;
  // To get the index in one byte
  uint32_t bitIndexInOneByte =bitIndex % 8;
  return static_cast<bool>
      (bitmap->bits[byteIndex] & (BITMAP_MASK << bitIndexInOneByte));
}

/*
  * A function to request memory allocation in the bit map
  * This function first brute-force find the byte which is not
  * `0xff`. If the `byteIndex` is lower than the length, this
  * function iteratively find the first zero bit. And the find
  * a continuous bit area to allocate the memory.

  * Actually, for myself, it is better to just allocate memory byte by
  * byte, alought there might be some space between the byte, but this
  * is a simple way.

  * param:
    1. bitmap struct
    2. the number of the bit
  * return:
    + If failed, return -1
    + Else return the start address index
*/
int bitmapScan(bitmap_t* bitmap, uint32_t count){
  uint32_t byteIndex = 0;
  while((0xff == bitmap->bits[byteIndex]) &&
    (byteIndex < bitmap->bitmapBytesLength)) {
    byteIndex++;
  }

  // If there is no space to allocate
  if(byteIndex == bitmap->bitmapBytesLength) {
    return -1;
  }

  int bitIndex = 0;
  while((uint8_t)((BITMAP_MASK << bitIndex) & bitmap->bits[byteIndex])) {
    bitIndex++;
  }
  int bitIndexStart = byteIndex * 8 + bitIndex;
  if(count == 1) {
    return bitIndexStart;
  }

  uint32_t bitLeft = bitmap->bitmapBytesLength * 8 - bitIndexStart;
  uint32_t nextBit = bitIndexStart + 1;
  uint32_t count_ = 1;

  bitIndexStart = -1;
  while(bitLeft-- > 0) {
    if(!(bitmapScanTest(bitmap, nextBit))) {
      count_++;
    } else {
      count_ = 0;
    }
    if(count == count_) {
      bitIndexStart = nextBit - count + 1;
      break;
    }
    nextBit++;
  }
  return bitIndexStart;
}

/*
  * A function to set the value of a bitmap

  * param:
    1. bitmap struct
    2. the bit index
    3. the value 0 or 1
  * return: void
*/
void bitmapSet(bitmap_t* bitmap, uint32_t bitIndex, int8_t value) {
  assert((value == 0) || (value == 1));
  uint32_t byteIndex = bitIndex / 8;
  uint32_t bitIndexInOneByte =bitIndex % 8;

  if(value) {
    bitmap->bits[byteIndex] |= (BITMAP_MASK << bitIndexInOneByte);
  } else {
    bitmap->bits[byteIndex] &= ~(BITMAP_MASK << bitIndexInOneByte);
  }
}
