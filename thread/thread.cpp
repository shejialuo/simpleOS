#include "stdint.hpp"
#include "string.hpp"
#include "global.hpp"
#include "memory.hpp"
#include "interrupt.hpp"
#include "debug.hpp"
#include "print.hpp"
#include "thread.hpp"

task_t* mainThread;
list_t threadReadyList;
list_t threadAllList;
static element_t* threadTag;

extern "C" void switchTo(task_t* current, task_t* next);

/*
  * A function to get the current PCB pointer
  * and make `esp` to `task_t`'s filed kernelStack.
  * Thus run the thread.
*/
task_t* runningThread() {
  uint32_t esp;
  asm ("mov %%esp, %0" : "=g" (esp));
  return (task_t*)(esp & 0xfffff000);
}

/*
  * A wrapper function to execute `threadFunction`.

  * param:
    1. threadFunction type is `void (void);`
    2. function argument
  * return: void
*/
static void kernelThread(threadFunction* function, void* functionArgument) {
  interruptionEnable();
  function(functionArgument);
}

/*
  * A funtion to initialize the thread stack and
  * put the function and function argument in the
  * thread stack in the specified position
*/
void threadCreate(task_t* pthread, threadFunction function, void* functionArgument) {

  /*
    * First, we should preserve the space of
    * the interruption stack.
  */
  pthread->kernelStack -= sizeof(istack_t);
  /*
    * Second, we should preserve the space of
    * the thread stack.
  */
  pthread->kernelStack -= sizeof(tstack_t);

  tstack_t * kThreadStack = reinterpret_cast<tstack_t*>(pthread->kernelStack);
  kThreadStack->eip = kernelThread;
  kThreadStack->function = function;
  kThreadStack->functionArgument = functionArgument;
  kThreadStack->ebp = kThreadStack->ebx = 0;
  kThreadStack->esi = kThreadStack->edi = 0;
}

/*
  * A function to initialize the thread, we have already
  * requested one page and get the pointer, so we initializes
  * the field one by one. The most important thing, we make
  * PCB kernelStack point to the page the highest address for
  * further use.

  * param:
    1. PCB
    2. the name of the task
    3. the priority of the task
  * return: void
*/
void initializationThread(task_t* pthread, char* name, int priority) {
  memset(pthread, 0, sizeof(*pthread));
  strcpy(pthread->name, name);

  if(pthread == mainThread) {
    pthread->status = TaskStatus::RUNNING;
  } else {
    pthread->status = TaskStatus::READY;
  }

  pthread->kernelStack = reinterpret_cast<uint32_t*>(
    reinterpret_cast<uint32_t>(pthread) + PG_SIZE
  );

  pthread->priority = priority;
  pthread->ticks = priority;
  pthread->elapsedTicks = 0;
  pthread->page = nullptr;
  /*
    * We need to make kernelStack points to the PCB highest address
  */

  pthread->stackMagic = 0x19870916;
}

/*
  * A function to start the thread. Firstly, it requests
  * a page of the memory, because PCB is page-size. And
  * it calls `initializationThread` and `threadCreate`
  * function to do some basic work
*/
task_t* threadStart(char* name, int priority, threadFunction function,
                    void* functionArgument) {

  /*
    * Now the thread is points to the Page low address,
    * so that's why in `initializationThread` we need to
    * make it at the page highest address.
  */

  task_t* thread = reinterpret_cast<task_t*>(getKernelPages(1));

  initializationThread(thread, name, priority);
  threadCreate(thread, function, functionArgument);

  assert(!elementFind(&threadReadyList, &thread->generalTag));
  // To add to the thread ready list
  listAppend(&threadReadyList, &thread->generalTag);

  assert(!elementFind(&threadAllList, &thread->allListTag));
  // To add to the thread all list
  listAppend(&threadAllList, &thread->allListTag);
  return thread;
}

/*
  * A function to set the `main` function to
  * become the main thread

  * param: void
  * return: void
*/
static void makeMainThread() {
  mainThread = runningThread();
  initializationThread(mainThread, "main", 31);

  assert(!elementFind(&threadAllList, &mainThread->allListTag));
  listAppend(&threadAllList, &mainThread->allListTag);
}

/*
  * A function to schedule the thread. It is invoked by the
  * time interruption. See `device/timer.cpp`. And it first
  * uses `runningThread` to get the current PCB and turns its
  * status to `TASKSTATUS::READY`. And Get the next thread from
  * the ready list. Use extern `switchTo` to finish schedule

  * param: void
  * return: void
*/
void schedule() {

  assert(interruptionGetStatus() == InterruptionStatus::OFF);

  task_t* current = runningThread();

  if(current->status == TaskStatus::RUNNING) {
    assert(!elementFind(&threadReadyList, &current->generalTag));
    listAppend(&threadReadyList, &current->generalTag);
    current->ticks = current->priority;
    current->status = TaskStatus::READY;
  } else {

  }

  assert(!listEmpty(&threadReadyList));
  threadTag = nullptr;

  threadTag = listPop(&threadReadyList);

  task_t* next= elem2entry(task_t, generalTag, threadTag);

  next->status = TaskStatus::RUNNING;
  switchTo(current, next);
}

/*
  * To initialize all the thread functionality

  * param: void
  * return: void
*/
void threadInitialization() {
  put_str("Thread initialization start\n");
  listInitialization(&threadReadyList);
  listInitialization(&threadAllList);
  makeMainThread();
  put_str("Thread initialization done\n");
}
