TI_GDT equ 0
RPL0 equ 0
SELECTOR_VIDEO equ (0x0003<<3) + TI_GDT + RPL0

section .data
put_int_buffer dq 0

[bits 32]
section .text
;---------------- put_str ----------------------
;put_str use put_char to print string
;-----------------------------------------------
global put_str
put_str:
  push ebx
  push ecx

  xor ecx, ecx
  mov ebx, [esp + 12]
.goon:
  mov cl, [ebx]
  ; '\0': string end
  cmp cl, 0
  jz .str_over
  push ecx
  call put_char
  add esp, 4
  inc ebx
  jmp .goon
.str_over:
  pop ecx
  pop ebx
  ret

;---------------- put_char ---------------------
;Functionality: print one character in the stack
;-----------------------------------------------
global put_char
put_char:
  ; To save all registers
  pushad
  mov ax, SELECTOR_VIDEO
  mov gs, ax

  ; Get the current cursor position

  mov dx, 0x03d4
  mov al, 0x0e
  out dx, al
  mov dx, 0x03d5
  in al, dx
  mov ah, al

  mov dx, 0x03d4
  mov al, 0x0f
  out dx, al
  mov dx, 0x03d5
  in al, dx

  ; Store the current cursor
  mov bx, ax

  ; The put char address is `esp + 36`
  ; Because there are four bytes return address
  ; and 32 bytes pushed registers content
  mov ecx, [esp + 36]

  cmp cl, 0xd
  jz .is_carriage_return
  cmp cl, 0xa
  jz .is_line_feed

  cmp cl, 0x08
  jz .is_backspace
  jmp .put_other

.is_backspace:
  dec bx
  shl bx, 1

  mov byte [gs:bx], 0x20
  mov byte [gs:bx], 0x07
  shr bx, 1
  jmp .set_cursor

.put_other:
  shl bx, 1
  mov [gs:bx], cl
  inc bx
  mov byte [gs:bx], 0x07
  shr bx, 1
  inc bx
  cmp bx, 2000
  jl .set_cursor

.is_line_feed:
.is_carriage_return:
  xor dx, dx
  mov ax, bx
  mov si, 80

  div si

  sub bx, dx

.is_carriage_return_end:
  add bx, 80
  cmp bx, 2000
.is_line_feed_end:
  jl .set_cursor

.roll_screen:
  cld
  mov ecx, 960
  mov esi, 0xb80a0
  mov edi, 0xb8000
  rep movsd


  mov ebx, 3840
  mov ecx, 80
.cls:
  mov word [gs:ebx], 0x0720
  add ebx, 2
  loop .cls
  mov bx,1920

.set_cursor:

  mov dx, 0x03d4
  mov al, 0x0e
  out dx, al
  mov dx, 0x03d5
  mov al, bh
  out dx, al


  mov dx, 0x03d4
  mov al, 0x0f
  out dx, al
  mov dx, 0x03d5
  mov al, bl
  out dx, al
.put_char_done:
  popad
  ret

;--- Get little endian to ASCII and reverse ----
;Input: number
;Output: print hex without `0x`
;-----------------------------------------------
global put_int
put_int:
  pushad
  mov ebp, esp
  mov eax, [ebp+36]
  mov edx, eax
  mov edi, 7
  mov ecx, 8
  mov ebx, put_int_buffer

.16based_4bits:
  and edx, 0x0000000F
  cmp edx, 9
  jg .is_A2F
  add edx, '0'
  jmp .store

.is_A2F:
   sub edx, 10
   add edx, 'A'

.store:
  mov [ebx+edi], dl
  dec edi
  shr eax, 4
  mov edx, eax
  loop .16based_4bits

.ready_to_print:
  inc edi
.skip_prefix_0:
  cmp edi,8
  je .full0
;找出连续的0字符, edi做为非0的最高位字符的偏移
.go_on_skip:
  mov cl, [put_int_buffer+edi]
  inc edi
  cmp cl, '0'
  je .skip_prefix_0
  dec edi
  jmp .put_each_num

.full0:
  mov cl,'0'

.put_each_num:
  push ecx
  call put_char
  add esp, 4
  inc edi
  mov cl, [put_int_buffer+edi]
  cmp edi,8
  jl .put_each_num
  popad
  ret

global set_cursor
set_cursor:
  pushad
  mov bx, [esp+36]
  mov dx, 0x03d4
  mov al, 0x0e
  out dx, al
  mov dx, 0x03d5
  mov al, bh
  out dx, al

  mov dx, 0x03d4
  mov al, 0x0f
  out dx, al
  mov dx, 0x03d5
  mov al, bl
  out dx, al
  popad

  ret
