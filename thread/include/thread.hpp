#ifndef _THREAD_THREAD_HPP
#define _THREAD_THREAD_HPP

#include "stdint.hpp"
#include "thread_t.hpp"

task_t* runningThread();
void threadCreate(task_t* pthread, threadFunction function, void* functionArgument);
void initializationThread(task_t* pthread, char* name, int priority);
task_t* threadStart(char* name, int priority, threadFunction function, void* functionArgument);
void schedule();
void threadInitialization();

#endif // _THREAD_THREAD_HPP
