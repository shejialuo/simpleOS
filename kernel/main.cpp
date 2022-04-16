#include "print.hpp"
#include "init.hpp"
#include "thread.hpp"
#include "interrupt.hpp"
#include "memory.hpp"
void threadTestA(void*);
void threadTestB(void*);

int main() {

  put_str("I am kernel\n");
  initializationAll();

  threadStart("threadTestA", 31, threadTestA, (void*)("argA "));
  threadStart("threadTestB", 8, threadTestB, (void*)("argB "));

  interruptionEnable();
  while(1) {
    put_str("Main ");
  }
  return 0;
}


void threadTestA(void* arg) {
  char* parameter = (char*)arg;
  while(1) {
    put_str(parameter);
  }
}

void threadTestB(void* arg) {
  char* parameter = (char*)arg;
  while(1) {
    put_str(parameter);
  }
}
