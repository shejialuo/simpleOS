#ifndef _THREAD_THREAD_T_HPP
#define _THREAD_THREAD_T_HPP

#include "stdint.hpp"
#include "list.hpp"

using threadFunction = void (void*);

enum class TaskStatus {
  RUNNING,
  READY,
  BLOCKED,
  WAITING,
  HANGING,
  DIED
};

/*
  * Interruption stack to protect the context
*/
struct InterruptionStack {
  uint32_t vectorNumber;
  uint32_t edi;
  uint32_t esi;
  uint32_t ebp;
  uint32_t espDummy;
  uint32_t ebx;
  uint32_t edx;
  uint32_t ecx;
  uint32_t eax;
  uint32_t gs;
  uint32_t fs;
  uint32_t es;
  uint32_t ds;

  /*
    * push when CPU from low privilege to
    * high privilege
  */
  uint32_t errorCode;
  void (*epi) ();
  uint32_t cs;
  uint32_t eflags;
  uint32_t ss;
};

/*
  * Thread has its own stack, in order to switch
  * the thread, we need this data structure to
  * store the context
*/
struct ThreadStack {
  uint32_t ebp;
  uint32_t ebx;
  uint32_t edi;
  uint32_t esi;

  /*
    * When the thread is executed first time, `eip`
    * points to the function which need to be called
    * And for other time, `eip` points to the switch
    * return address
  */
  void (*eip)(threadFunction function, void* funcionArgument);

  void (*unusedReturnAddress);
  threadFunction * function;
  void* functionArgument;
};

/*
  * PCB definition
*/
struct TaskStruct {
  uint32_t* kernelStack; // a pointer which points to task stack pointer.
  TaskStatus status;     // the task status
  char name[16];         // the task name
  uint8_t priority;      // the task priority
  uint8_t ticks;
  uint32_t elapsedTicks;
  element_t generalTag;
  element_t allListTag;
  uint32_t* page;
  uint32_t stackMagic;   // the magic
};

using istack_t = struct InterruptionStack;
using tstack_t = struct ThreadStack;
using task_t = struct TaskStruct;

#endif // _THREAD_THREAD_T_HPP
