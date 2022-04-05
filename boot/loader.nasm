%include "boot.inc"
section loader vstart=LOADER_BASE_ADDR
LOADER_STACK_TOP equ LOADER_BASE_ADDR
jmp loader_start

; build gdt
GDT_BASE: dd 0x00000000
          dd 0x00000000
CODE_DESC: dd 0x0000FFFF
           dd DESC_CODE_HIGH4
DATA_STACK_DESC: dd 0x0000FFFF
                 dd DESC_DATA_HIGH4
VEDIO_DESC: dd 0x80000007
            dd DESC_VIDEO_HIGH4

GDT_SIZE equ $ - GDT_BASE
GDT_LIMIT equ GDT_SIZE - 1
times 60 dq 0

SELECTOR_CODE equ (0x0001<<3) + TI_GDT + RPL0
SELECTOR_DATA equ (0x0002<<3) + TI_GDT + RPL0
SELECTOR_VIDEO equ (0x0003<<3) + TI_GDT + RPL0

; `total_mem_bytes` is used to store the capacity of the memory.
; The address is `0xb03`.
total_mem_bytes dd 0

gdt_ptr dw GDT_LIMIT
        dd GDT_BASE

; Memory alignment
ards_buf times 244 db 0
ards_nr dw 0

loader_start:

  ; Memory access method 1
  ; int 15h eax = 0000E820H, edx = 534D4150H
  xor ebx, ebx
  mov edx, 0x534d4150
  mov di, ards_buf
.e820_mem_get_loop:
  mov eax, 0x0000e820
  mov ecx, 20
  int 0x15
  jc .e820_failed_so_try_e801
  add di, cx
  inc word [ards_nr]
  cmp ebx, 0
  jnz .e820_mem_get_loop

  ; In all the structure of `ards_buf`
  ; find the max `base_add_low + length_low`
  mov cx, [ards_nr]
  mov ebx, ards_buf
  xor edx, edx

.find_max_mem_area:
  mov eax, [ebx]
  add eax, [ebx + 8]
  add ebx, 20
  cmp edx, eax
  jge .next_ards
  mov edx, eax
.next_ards:
  loop .find_max_mem_area
  jmp .mem_get_ok

  ; Memory access method 2
.e820_failed_so_try_e801:
  mov ax, 0xe801
  int 0x15
  jc .e801_failed_so_try88

  mov cx, 0x400
  mul cx
  shl edx, 16
  and eax, 0x0000FFFF
  or edx, eax
  add ebx, 0x100000
  mov esi, edx

  xor eax, eax
  mov ax, bx
  mov ecx, 0x10000
  mul ecx

  add esi, eax

  mov edx, esi
  jmp .mem_get_ok

.e801_failed_so_try88:
  mov ah, 0x88
  int 0x15
  jc .error_hlt
  and eax, 0x0000FFFF

  mov cx, 0x400
  mul cx
  shl edx, 16
  or edx, eax
  add ebx, 0x100000

.error_hlt:
  jmp $

.mem_get_ok:
  mov [total_mem_bytes], edx

; --------- protected mode -----------------
  in al, 0x92
  or al, 0000_0010B
  out 0x92, al

  lgdt [gdt_ptr]

  mov eax, cr0
  or eax, 0x00000001
  mov cr0, eax

  ; flush cpu pipeline
  jmp dword SELECTOR_CODE:p_mode_start

[bits 32]
p_mode_start:
  mov ax, SELECTOR_DATA
  mov ds, ax
  mov es, ax
  mov ss, ax
  mov esp, LOADER_STACK_TOP
  mov ax, SELECTOR_VIDEO
  mov gs, ax

  call setup_page

  sgdt [gdt_ptr]
  mov ebx, [gdt_ptr + 2]
  or dword [ebx + 0x18 + 4], 0xc0000000

  add dword [gdt_ptr + 2], 0xc0000000
  add esp, 0xc0000000

  mov eax, PAGE_DIR_TABLE_POS
  mov cr3, eax

  mov eax, cr0
  or eax,  0x80000000
  mov cr0, eax

  lgdt [gdt_ptr]

  mov byte [gs:160], 'V'

  jmp $

;-------- Create page directory and page -------
; Here we put the page directory at 0x100000,
; descirbed by `PAGE_DIR_TABLE_POS` in `boot.inc`.
setup_page:
  mov ecx, 4096
  mov esi, 0
.clear_page_dir:
  ; Because page directory is 4kb, so use loop
  ; to make page directory be 0 byte by byte
  mov byte [PAGE_DIR_TABLE_POS + esi], 0
  inc esi
  loop .clear_page_dir
.create_pde:
  ; Here we make a 4G virtual address space.
  ; The OS occupies 1G of high address, so we
  ; need to make the address which is bleow
  ; `0xc03fffff` and `0x003fffff` to points to
  ; the same page table entry
  mov eax, PAGE_DIR_TABLE_POS
  add eax, 0x1000
  mov ebx, eax

  or eax, PG_US_U | PG_RW_W | PG_P

  ; We let page directory table
  ; item `0` and `0xc00` to points to the same page
  ; table entry.
  mov [PAGE_DIR_TABLE_POS + 0x0], eax
  mov [PAGE_DIR_TABLE_POS+ 0xc00], eax

  ; Make the last page directory table entry points to
  ; page directory table itself
  sub eax, 0x1000
  mov [PAGE_DIR_TABLE_POS + 4092], eax

  ; Because the actualy physical memory for kernel is 1M
  ; and the page size is 4k, so we need 256 pages for kernel.
  mov ecx, 256 ; 1M / 4k = 256
  mov esi, 0
  mov edx, PG_US_U | PG_RW_W | PG_P
.create_pte:
  mov [ebx + esi * 4], edx
  ; Here, we use `edx` to store the page table entry phsyical
  ; address, thus make virtual address equal phsyical address
  add edx, 4096
  inc esi
  loop .create_pte

  mov eax, PAGE_DIR_TABLE_POS
  add eax, 0x2000
  or eax, PG_US_U | PG_RW_W | PG_P
  mov ebx, PAGE_DIR_TABLE_POS
  mov ecx, 254
  mov esi, 769

.create_kernel_pde:
  mov [ebx+esi*4], eax
  inc esi
  add eax, 0x1000
  loop .create_kernel_pde

  ret