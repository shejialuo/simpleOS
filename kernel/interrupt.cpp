#include "interrupt.hpp"
#include "stdint.hpp"
#include "interrupt_t.hpp"
#include "global.hpp"
#include "io.hpp"
#include "print.hpp"

static igd_t igd[INTERRUPTION_NUM];

#define GET_EFLAGS(EFLAG_VAR) asm volatile("pushfl; popl %0" : "=g" (EFLAG_VAR))

/*
  * A function to create an interruption Gate Descrptor

  * param:
    1. a pointer which points to `InterruptionGateDescriptor`
    2. attribute of the `InterruptionGateDescriptor`
    3. interruptition function
  * return: void
*/
static void createInterruptionGateDescriptor(
  igd_t * igd, uint8_t attribute, interruptHandler function) {
  igd->functionOffsetLowAddress = (uint32_t)(function) & 0x0000FFFF;
  igd->selector = SELECTOR_K_CODE;
  igd->unusedByte = 0;
  igd->attribute = attribute;
  igd->functionOffsetHighAddress = ((uint32_t)(function) & 0xFFFF0000) >> 16;
}

/*
  * A function iteratively initialize all the
  * interruption gate descriptors

  * param:  void
  * return: void
*/
static void buildAllInterruptionGateDescriptors() {
  for(int16_t i = 0; i < INTERRUPTION_NUM; ++i) {
    createInterruptionGateDescriptor(
      &igd[i], IGD_ATTRIBUTE_DPL0 ,interruptionFunctionEntryTable[i]);
  }
  put_str("InterruptionGateDescriptors initialization done\n");
}

/*
  * A function which happens when some general interruption
  * happening, especially exception.

  * param: interruption vector number
  * return: void
*/
static void generalInterruptionHandler(uint8_t vectorNumber) {
  if(vectorNumber == 0x27 || vectorNumber == 0x2f) {
    return;
  }
  set_cursor(0);
  int cursorPosition = 0;
  while(cursorPosition < 320) {
    put_char(' ');
    cursorPosition++;
  }

  set_cursor(0);
  put_str("!!!!!!!      excetion message begin  !!!!!!!!\n");
  set_cursor(88);
  put_str(interruptionName[vectorNumber]);
  if(vectorNumber == 14) {
    int pageFaultVirtualAddress = 0;
    asm ("movl %%cr2, %0" : "=r" (pageFaultVirtualAddress));
    put_str("\npage fault addr is ");
    put_int(pageFaultVirtualAddress);
  }
  put_str("\n!!!!!!!      excetion message end    !!!!!!!!\n");
  while(1) ;
}

/*
  * A function to register the interruption function

  * param:
    1. interruption vector number
    2. inteeruption function
  * return: void
*/
void registerHandler(uint8_t vectorNumber, interruptHandler function) {
  interruptionFunctionTable[vectorNumber] = function;
}

/*
  * A function which basically initializes `interruptionFunctionTable`
  * and add exception name.

  * param: void
  * return: void
*/
static void exceptionInitialization() {
  for(int i = 0; i < INTERRUPTION_NUM; ++i) {
    /*
      * All the interruption function to be `generalInterruptionHandler`,
      * registered after.
    */
    interruptionFunctionTable[i] = reinterpret_cast<void*>(generalInterruptionHandler);
    interruptionName[i] = "unknown";
  }
  interruptionName[0] = "#DE Divide Error";
  interruptionName[1] = "#DB Debug Exception";
  interruptionName[2] = "NMI Interrupt";
  interruptionName[3] = "#BP Breakpoint Exception";
  interruptionName[4] = "#OF Overflow Exception";
  interruptionName[5] = "#BR BOUND Range Exceeded Exception";
  interruptionName[6] = "#UD Invalid Opcode Exception";
  interruptionName[7] = "#NM Device Not Available Exception";
  interruptionName[8] = "#DF Double Fault Exception";
  interruptionName[9] = "Coprocessor Segment Overrun";
  interruptionName[10] = "#TS Invalid TSS Exception";
  interruptionName[11] = "#NP Segment Not Present";
  interruptionName[12] = "#SS Stack Fault Exception";
  interruptionName[13] = "#GP General Protection Exception";
  interruptionName[14] = "#PF Page-Fault Exception";
  // interruptionName[15] unused
  interruptionName[16] = "#MF x87 FPU Floating-Point Error";
  interruptionName[17] = "#AC Alignment Check Exception";
  interruptionName[18] = "#MC Machine-Check Exception";
  interruptionName[19] = "#XF SIMD Floating-Point Exception";
}

/*
  * A function uses `lib/kernel/io.hpp` to
  * deal with 8529A

  * param:  void
  * return: void
*/
static void PICInitialization() {

  outb (PIC_M_CTRL, 0x11);
  outb (PIC_M_DATA, 0x20);
  outb (PIC_M_DATA, 0x04);
  outb (PIC_M_DATA, 0x01);

  outb (PIC_S_CTRL, 0x11);
  outb (PIC_S_DATA, 0x28);
  outb (PIC_S_DATA, 0x02);
  outb (PIC_S_DATA, 0x01);

  outb (PIC_M_DATA, 0xfe);
  outb (PIC_S_DATA, 0xff);

  put_str("PIC initialization done\n");
}

/*
  * A function to apply interruption initialization which
  * executes the intrruption gate descriptors initialization
  * and initialize 8259A. At last, use `lidt` to write to IDTR

  * param:  void
  * return: void
*/
void interruptionInitialization() {
  put_str("Interruption initialization start\n");

  buildAllInterruptionGateDescriptors();
  exceptionInitialization();

  PICInitialization();

  uint64_t igdSize = ((sizeof(igd) - 1) | ((uint64_t)((uint32_t)igd << 16)));
  asm volatile("lidt %0" ::"m" (igdSize));

  put_str("Interruption initialization done\n");
}

/*
  * A function to enable interruption and return
  * the old interruption status

  * param: void
  * return: scoped enum class `InterruptionStatus`
*/
InterruptionStatus interruptionEnable() {
  InterruptionStatus oldStatus;
  if (InterruptionStatus::ON == interruptionGetStatus()) {
    oldStatus == InterruptionStatus::ON;
    return oldStatus;
  } else {
    oldStatus = InterruptionStatus::OFF;
    asm volatile("sti");
    return oldStatus;
  }
}

/*
  * A function to disable interruption and return
  * the old interruption status

  * param: void
  * return: scoped enum class `InterruptionStatus`
*/
InterruptionStatus interruptionDisable() {
  InterruptionStatus oldStatus;
  if (InterruptionStatus::ON == interruptionGetStatus()) {
    oldStatus == InterruptionStatus::ON;
    asm volatile("cli" ::: "memory");
    return oldStatus;
  } else {
    oldStatus = InterruptionStatus::OFF;
    return oldStatus;
  }
}

/*
  * A function to set the interruption status

  * param: scoped enum class `InterruptionStatus`
  * return: scoped enum class `InterruptionStatus`
*/
InterruptionStatus interruptionSetStatus(InterruptionStatus status) {
  return (status == InterruptionStatus::ON) ? interruptionEnable() : interruptionDisable();
}

/*
  * A function to get current interruption status

  * param: void
  * return: scoped enum class `InterruptionStatus`
*/
InterruptionStatus interruptionGetStatus() {
  uint32_t eflags = 0;
  GET_EFLAGS(eflags);
  return (EFLAGS_IF & eflags) ? InterruptionStatus::ON : InterruptionStatus::OFF;
}
