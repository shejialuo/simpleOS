#ifndef _KERNEL_INTERRUPT_HPP
#define _KERNEL_INTERRUPT_HPP

#include "stdint.hpp"

using interruptHandler = void*;

void interruptionInitialization();

/*
  * Define the two states of interruption
  * 1. ON
  * 2. OFF
*/
enum class InterruptionStatus {
  ON,
  OFF
};

InterruptionStatus interruptionEnable();
InterruptionStatus interruptionDisable();
InterruptionStatus interruptionSetStatus(InterruptionStatus);
InterruptionStatus interruptionGetStatus();
void registerHandler(uint8_t vectorNumber, interruptHandler function);

#endif // _KERNEL_INTERRUPT_HPP
