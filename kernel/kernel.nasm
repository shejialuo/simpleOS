[bits 32]

; Because CPU has already pushed the ERROR_CODE
; So use `nop`
%define ERROR_CODE nop

; To unify the format of the stack, push zero manually
%define ZERO push 0

extern interruptionFunctionTable

section .data
global interruptionFunctionEntryTable
interruptionFunctionEntryTable:

%macro VECTOR 2
section .text
intr%1entry:
  %2 ; %2 is ZERO

  ; To save the context
  push ds
  push es
  push fs
  push gs
  pushad

  mov al, 0x20
  out 0xa0, al ; To send to the slave
  out 0x20, al ; To send to the master

  push %1

  ; call the C program
  call [interruptionFunctionTable + %1*4]
  jmp interruptionExit

section .data
  dd intr%1entry ; To store the address of the program
%endmacro

section .text
global interruptionExit
interruptionExit:
  ; To restore the context
  add esp, 4
  popad
  pop gs
  pop fs
  pop es
  pop ds
  add esp, 4
  iretd

VECTOR 0x00, ZERO
VECTOR 0x01, ZERO
VECTOR 0x02, ZERO
VECTOR 0x03, ZERO
VECTOR 0x04, ZERO
VECTOR 0x05, ZERO
VECTOR 0x06, ZERO
VECTOR 0x07, ZERO
VECTOR 0x08, ERROR_CODE
VECTOR 0x09, ZERO
VECTOR 0x0a, ERROR_CODE
VECTOR 0x0b, ERROR_CODE
VECTOR 0x0c, ZERO
VECTOR 0x0d, ERROR_CODE
VECTOR 0x0e, ERROR_CODE
VECTOR 0x0f, ZERO
VECTOR 0x10, ZERO
VECTOR 0x11, ERROR_CODE
VECTOR 0x12, ZERO
VECTOR 0x13, ZERO
VECTOR 0x14, ZERO
VECTOR 0x15, ZERO
VECTOR 0x16, ZERO
VECTOR 0x17, ZERO
VECTOR 0x18, ERROR_CODE
VECTOR 0x19, ZERO
VECTOR 0x1a, ERROR_CODE
VECTOR 0x1b, ERROR_CODE
VECTOR 0x1c, ZERO
VECTOR 0x1d, ERROR_CODE
VECTOR 0x1e, ERROR_CODE
VECTOR 0x1f, ZERO
VECTOR 0x20, ZERO
