#include "print.hpp"
#include "init.hpp"

int main() {

  put_str("I am kernel\n");
  initializationAll();
  asm volatile("sti");
  while(1);
  return 0;
}
