#include "bitmap.hpp"
#include "stdint.hpp"
#include "global.hpp"
#include "debug.hpp"
#include "print.hpp"
#include "string.hpp"
#include "memory.hpp"

pool_t kernelPool, userPool;
vaddress_t kernelVirtualAddress;

constexpr uint32_t PDE_IDX(uint32_t address) {
  return ((address & 0xffc00000) >> 22);
}

constexpr int32_t PTE_IDX(int32_t address) {
  return ((address & 0x003ff000) >> 12);
}

/*
  * A function to request `pageCount` number of virtual page.
  * If suceessful, return the `virtualAddressStart`,
  * Else return nullptr;

  * param:
    1. PoolFlag indicates whether is kernel memory pool or user memory pool
    2. page number needed to request
  * return:
    + If sucessful, return the start address
    + Else return nullptr
*/
static void* virtualAddressGet(PoolFlag poolFlag, uint32_t pageCount) {
  int virtualAddressStart = 0, bitIndexStart = -1;
  uint32_t count = 0;
  if (poolFlag == PoolFlag::KERNEL) {
    bitIndexStart = bitmapScan(&kernelVirtualAddress.virtualBitmap, pageCount);
    if(bitIndexStart == -1) {
      return nullptr;
    }
    while(count < pageCount) {
      bitmapSet(&kernelVirtualAddress.virtualBitmap, bitIndexStart+count, 1);
      count++;
    }
    virtualAddressStart = kernelVirtualAddress.virtualAddressStart +
      bitIndexStart * PG_SIZE;
  } else {
    // TODO: user memory pool
  }
  return reinterpret_cast<void*>(virtualAddressStart);
}

/*
  * A function to get the virtual address's page directory
  * entry pointer.

  * param: virtual address
  * return: the pointer points to the page directory entry
*/
uint32_t* pageDirectoryEntryPointer(uint32_t virtualAddress) {
  uint32_t* pde = (uint32_t*)((0xfffff000) + PDE_IDX(virtualAddress) * 4);
  return pde;
}

/*
  * A function to get virtual address's page table entry
  * pointer.

  * param: virtual address
  * return: the pointer points to the page table entry
*/
uint32_t* pageTableEntryPointer(uint32_t virtualAddress) {
  uint32_t* pte = (uint32_t*)(0xffc00000 +
    ((virtualAddress & 0xffc00000) >> 10) + PTE_IDX(virtualAddress) * 4);
  return pte;
}

/*
  * A function to allocate one physical page in the memory pool
  * If successful, return the physical address
  * Else return nullptr
*/
static void* poolAllocate(pool_t* pool) {
  int bitIndex = bitmapScan(&pool->poolBitmap, 1);
  if(bitIndex == -1) {
    return nullptr;
  }
  bitmapSet(&pool->poolBitmap, bitIndex, 1);
  uint32_t pagePhysicalAddress = ((bitIndex * PG_SIZE) +
    pool->physicalAddressStart);
  return reinterpret_cast<void*>(pagePhysicalAddress);
}

/*
  * A function to add page table and add the map
  * virtual address to physical address

  * param:
    1. virtual address
    2. physical address
  * return: void
*/
static void pageTableAdd(void* virtualAddress_, void* pagePhysicalAddress_) {
  uint32_t virutalAddress = reinterpret_cast<uint32_t>(virtualAddress_);
  uint32_t pagePhysicalAddress = reinterpret_cast<uint32_t>(pagePhysicalAddress_);
  uint32_t* pde = pageDirectoryEntryPointer(virutalAddress);
  uint32_t* pte = pageTableEntryPointer(virutalAddress);

  if(*pde & 0x00000001) {
    assert(!(*pte & 0x00000001));
    if(!(*pte & 0x00000001)) {
      *pte = (pagePhysicalAddress | PG_US_U | PG_RW_W | PG_P_1);
    } else {
      PANIC("pte repeat");
      *pte = (pagePhysicalAddress | PG_US_U | PG_RW_W | PG_P_1);
    }
  } else {
    uint32_t pdePhysicalAddress = reinterpret_cast<uint32_t>(
      poolAllocate(&kernelPool)
    );
    *pde = (pdePhysicalAddress | PG_US_U | PG_RW_W | PG_P_1);
    memset((void*)((int)pte & 0xfffff000), 0, PG_SIZE);

    assert(!(*pte & 0x00000001));
    *pte = (pagePhysicalAddress | PG_US_U | PG_RW_W | PG_P_1);
  }
}

/*
  * A function to request page finally

  * param:
    1. PoolFlag indicates whether is kernel memory pool or user memory pool
    2. page number needed to request
  * return: a pointer points to the start address
*/
void* mallocPage(PoolFlag poolFlag, uint32_t pageCount) {
  assert(pageCount > 0 && pageCount < 3840);

  /*
    * There are three steps to do page malloc
    * 1. use `virtualAddressGet` tp request virtual address in
    *    the virtual memory pool
    * 2. use `poolAllocate` to request physical page in the
    *    physical pool address
    * 3. use `pageTableAdd` to finish mapping.
  */

  void* virtualAddressStart = virtualAddressGet(poolFlag, pageCount);
  if(virtualAddressStart == nullptr) {
    return nullptr;
  }
  uint32_t virtualAddress = reinterpret_cast<uint32_t>(virtualAddressStart);
  uint32_t count = pageCount;

  pool_t* memoryPool = (poolFlag == PoolFlag::KERNEL) ? &kernelPool : &userPool;
  while(count-- > 0) {
    void* pagePhysicalAddress = poolAllocate(memoryPool);
    if(pagePhysicalAddress == nullptr) {
      // TODO: rollback
      return nullptr;
    }
    pageTableAdd(reinterpret_cast<void*>(virtualAddress), pagePhysicalAddress);
    virtualAddress += PG_SIZE;
  }
  return virtualAddressStart;
}

/*
  * A function to request page memory

  * param: page need to request
  * return: a pointer points to the start address
*/
void* getKernelPages(uint32_t pageCount) {
  void* virtualAddress = mallocPage(PoolFlag::KERNEL, pageCount);
  if(virtualAddress != nullptr) {
    memset(virtualAddress, 0, pageCount * PG_SIZE);
  }
  return virtualAddress;
}

/*
  * A function to do memory pool initialization

  * param: void
  * return: void
*/
static void memoryPoolInitialization(uint32_t allMemory) {
  put_str("Memory pool initialization start\n");
  /*
    * `pageTableSize` is used to recored the page directory
    * and page table size, because we make 0th and 768th
    * page directory to point to the same page, and 769th-
    * 1002th points to 254 page tables, so there are 256th
    * page tables.
  */
  uint32_t pageTableSize = PG_SIZE * 256;
  /*
    * `0x100000` is the low 1M memory, which kernel used.
    * we don't care about this.
  */
  uint32_t usedMemory = pageTableSize + 0x100000;
  uint32_t freeMemory = allMemory - usedMemory;

  uint16_t allFreePages = freeMemory / PG_SIZE;
  uint16_t kernelFreePages = allFreePages / 2;
  uint16_t userFreePages = allFreePages - kernelFreePages;

  /*
    * Here, we don't consider any remainder memory
  */
  uint32_t kernelBitmapLength = kernelFreePages / 8;
  uint32_t userBitmapLength = userFreePages / 8;

  uint32_t kernelPageStart = usedMemory;
  uint32_t userPageStart = kernelPageStart + kernelFreePages * PG_SIZE;

  kernelPool.physicalAddressStart = kernelPageStart;
  userPool.physicalAddressStart = userPageStart;

  kernelPool.poolSize = kernelFreePages * PG_SIZE;
  userPool.poolSize = userFreePages * PG_SIZE;

  kernelPool.poolBitmap.bitmapBytesLength = kernelBitmapLength;
  userPool.poolBitmap.bitmapBytesLength = userBitmapLength;

  kernelPool.poolBitmap.bits = reinterpret_cast<uint8_t*>(MEM_BITMAP_BASE);
  userPool.poolBitmap.bits = reinterpret_cast<uint8_t*>
    (MEM_BITMAP_BASE + kernelBitmapLength);

  put_str("Kernel pool bitmap initialization start:");
  put_int(reinterpret_cast<int>(kernelPool.poolBitmap.bits));
  put_str("\n");
  put_str("Kernel pool physical address start:");
  put_int(kernelPool.physicalAddressStart);
  put_str("\n");
  put_str("User pool bitmap start:");
  put_int(reinterpret_cast<int>(userPool.poolBitmap.bits));
  put_str("\n");
  put_str("User pool physical address start:");
  put_int(userPool.physicalAddressStart);
  put_str("\n");

  bitmapInitialization(&kernelPool.poolBitmap);
  bitmapInitialization(&userPool.poolBitmap);

  kernelVirtualAddress.virtualBitmap.bitmapBytesLength = kernelBitmapLength;
  // For now, the virtual address bitmap start address is located
  // away from kernel pool nad user pool
  kernelVirtualAddress.virtualBitmap.bits = reinterpret_cast<uint8_t*>(
    MEM_BITMAP_BASE + kernelBitmapLength + userBitmapLength);

  kernelVirtualAddress.virtualAddressStart = K_HEAP_START;
  bitmapInitialization(&kernelVirtualAddress.virtualBitmap);
  put_str("Memory pool initialization done\n");
}

/*
  * A function to initialize memory

  * param: void
  * return: void
*/
void memoryInitialization() {
  put_str("Memory initialization start\n");
  // `0xb03` is the memory we set in the assembly code
  uint32_t memoryBytesTotal = (*(uint32_t*)(0xb03));
  memoryPoolInitialization(memoryBytesTotal);
  put_str("Memory initialization done\n");
}
