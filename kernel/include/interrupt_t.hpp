#ifndef _KERNEL_INTERRUPT_T_HPP
#define _KERNEL_INTERRUPT_T_HPP

/*
  * This file is used to define the type and
  * some constant which `interrupt.cpp` will
  * use.
*/

#include "stdint.hpp"
#include "interrupt.hpp"

/*
  * Constants definition
  ! Here, use `constexpr` to do compile-time calculation
*/

constexpr uint16_t INTERRUPTION_NUM = 0x21; // The support number of interruption
constexpr uint16_t PIC_M_CTRL = 0x20;       // 8259A's main control port
constexpr uint16_t PIC_M_DATA = 0x21;       // 8259A's main data port
constexpr uint16_t PIC_S_CTRL = 0xa0;       // 8259A's slave control port
constexpr uint16_t PIC_S_DATA = 0xa1	;      // 8259A's slave data port

constexpr uint16_t EFLAGS_IF = 0x00000200;  // Eflags if

/*
  * To define the interuppt gate descriptor
  * from low address to high address
*/
struct InterruptGateDescriptor{
  uint16_t functionOffsetLowAddress;
  uint16_t selector;
  uint8_t  unusedByte;
  uint8_t  attribute;
  uint16_t functionOffsetHighAddress;
};

/*
  * `igd_t` is a type alias to `InterruptGateDescriptor`
*/
using igd_t = struct InterruptGateDescriptor;

/*
  * `interruptionFunctionTable` is used to store the actuall
  * interruption function address
*/
interruptHandler interruptionFunctionTable[INTERRUPTION_NUM];

/*
  * `interruptionEntryTable` is defined at `kernel.nasm`,
  * which will execute high-level code for interruption.
*/
extern interruptHandler interruptionFunctionEntryTable[INTERRUPTION_NUM];

/*
  * `interruptionName` is used to store the exception name
*/
char* interruptionName[INTERRUPTION_NUM];

#endif // _KERNEL_INTERRUPT_T_HPP
