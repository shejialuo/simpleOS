#include "debug.hpp"
#include "print.hpp"
#include "interrupt.hpp"

/*
  * A function which prints the filename, the
  * line number and the function name and
  * make the program spin

  * param:
    1. filename
    2. line number
    3. function name
    4. condition
  * return: void
*/
void panicSpin(char* filename, int line,
  const char* functionName, const char* condition) {
  interruptionDisable();
  put_str("\n\n\n!!!!! error !!!!!\n");
  put_str("filename:");put_str(filename);put_str("\n");
  put_str("line:0x");put_int(line);put_str("\n");
  put_str("function:");put_str((char*)functionName);put_str("\n");
  put_str("condition:");put_str((char*)condition);put_str("\n");
  while(true) ;
}
