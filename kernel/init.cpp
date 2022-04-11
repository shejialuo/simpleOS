#include "init.hpp"
#include "print.hpp"
#include "interrupt.hpp"
#include "timer.hpp"
#include "memory.hpp"

/*
  * A function to initialize all the components

  * param: void
  * return: void
*/
void initializationAll() {
  put_str("Initialization all\n");
  interruptionInitialization();
  timerInitialization();
  memoryInitialization();
}
