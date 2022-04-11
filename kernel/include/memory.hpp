#ifndef _KERNEL_MEMORY_HPP
#define _KERNEL_MEMORY_HPP

#include "memory_t.hpp"

void memoryInitialization();
void* getKernelPages(uint32_t pageCount);
void* mallocPage(PoolFlag poolFlag, uint32_t pageCount);
uint32_t* pageTableEntryPointer(uint32_t virtualAddress);
uint32_t* pageDirectoryEntryPointer(uint32_t virtualAddress);

#endif // _KERNEL_MEMORY_HPP
