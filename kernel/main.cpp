#include "print.hpp"
#include "init.hpp"
#include "memory.hpp"

int main() {

  put_str("I am kernel\n");
  initializationAll();
  void* address = getKernelPages(3);
  put_str("\n get_kernel_page start vaddr is ");
  put_int(reinterpret_cast<uint32_t>(address));
  put_str("\n");
  while(1);
  return 0;
}
