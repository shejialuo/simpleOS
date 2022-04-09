#ifndef _KERNEL_GLOBAL_HPP
#define _KERNEL_GLOBAL_HPP

#include "stdint.hpp"

constexpr uint8_t RPL0 = 0;
constexpr uint8_t RPL1 = 1;
constexpr uint8_t RPL2 = 2;
constexpr uint8_t RPL3 = 3;

constexpr uint8_t TI_GDT = 0;
constexpr uint8_t TI_LDT = 1;

constexpr uint16_t SELECTOR_K_CODE = (1 << 3) + (TI_GDT << 2) + RPL0;
constexpr uint16_t SELECTOR_K_DATA = (2 << 3) + (TI_GDT << 2) + RPL0;
constexpr uint16_t SELECTOR_K_STACK = SELECTOR_K_DATA;
constexpr uint16_t SELECTOR_k_GS = (3 << 3) + (TI_GDT << 2) + RPL0;

/*
  * Interruption descriptor attribute
*/

constexpr uint8_t IGD_P = 1;
constexpr uint8_t IGD_DPL0 = 0;
constexpr uint8_t IGD_DPL3 = 3;

constexpr uint8_t IGD_32_TYPE = 0xE;

constexpr uint8_t IGD_ATTRIBUTE_DPL0 =
  (IGD_P << 7) + (IGD_DPL0 << 5) + IGD_32_TYPE;
constexpr uint8_t IDT_ATTRIBUTE_DPL3 =
  (IGD_P << 7) + (IGD_DPL3 << 5) + IGD_32_TYPE;

#endif // _KERNEL_GLOBAL_HPP
