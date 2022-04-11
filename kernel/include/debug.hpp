#ifndef _KERNEL_DEBUG_HPP
#define _KERNEL_DEBUG_HPP

enum class Debug {
  ON,
  OFF
};

void panicSpin(char* , int , const char* , const char* );

#define PANIC(...) panicSpin(__FILE__, __LINE__, __func__, __VA_ARGS__)

#ifdef NDEBUG
   #define assert(CONDITION) ((void)0)
#else
   #define assert(CONDITION)           \
      if (CONDITION) {} else {         \
  		                                 \
	 PANIC(#CONDITION);                  \
      }
#endif /*__NDEBUG */

#endif // _KERNEL_DEBUG_HPP
