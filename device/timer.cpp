#include "timer.hpp"
#include "io.hpp"
#include "print.hpp"

constexpr uint8_t IRQ0_FREQUENCY = 100;
constexpr uint32_t INPUT_FREQUENCY = 1193180;
constexpr uint32_t COUNTER0_VALUE = INPUT_FREQUENCY / IRQ0_FREQUENCY;
constexpr uint16_t CONTRER0_PORT = 0x40;
constexpr uint8_t COUNTER0_NO = 0;
constexpr uint8_t COUNTER_MODE = 2;
constexpr uint8_t READ_WRITE_LATCH = 3;
constexpr uint16_t PIT_CONTROL_PORT = 0x43;

/* 把操作的计数器counter_no、读写锁属性rwl、计数器模式counter_mode写入模式控制寄存器并赋予初始值counter_value */
static void frequencySet(uint8_t counter_port,uint8_t counter_no,
  uint8_t rwl, uint8_t counter_mode, uint16_t counter_value) {
   outb(PIT_CONTROL_PORT, (uint8_t)(counter_no << 6 | rwl << 4 | counter_mode << 1));
   outb(counter_port, (uint8_t)counter_value);
   outb(counter_port, (uint8_t)counter_value >> 8);
}

/*
  * A function to initialize the timer

  * param: void
  * return: void
*/
void timerInitialization() {
   put_str("Timer initialization start\n");
   frequencySet(CONTRER0_PORT, COUNTER0_NO, READ_WRITE_LATCH, COUNTER_MODE, COUNTER0_VALUE);
   put_str("Timer initialization done\n");
}
