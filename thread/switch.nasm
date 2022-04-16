[bits 32]
section .text
global switchTo
switchTo:
  ; To obey the ABI rule
  push esi
  push edi
  push ebx
  push ebp

  ; Now the `eax` points to the `current` pcb
  mov eax, [esp + 20]
  ; We store the `esp` to `task_t`'s kernelStack
  mov [eax], esp

  ; Now the `eax` points to the `next `pcb`
  mov eax, [esp + 24]
  ; We make the `esp` be `task_t`'s kernelStack
  mov esp, [eax]

  pop ebp
  pop ebx
  pop edi
  pop esi

  ret
