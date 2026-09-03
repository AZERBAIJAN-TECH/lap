BITS 32
ORG 0x8000

; ---- точка входа: должна быть первым байтом блоба ----
rm_set_video_mode:
    pushad
    cli
    sidt [saved_idtr]
    mov [saved_esp32], esp

    jmp CODE16_SEG:pm16_start

BITS 16
pm16_start:
    mov ax, DATA16_SEG
    mov ds, ax
    mov es, ax
    mov ss, ax

    mov eax, cr0
    and eax, ~1
    mov cr0, eax

    jmp 0x0000:rm_start

rm_start:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    lidt [real_idtr]
    sti

    call vbe_find_and_set_mode

    cli
    lgdt [local_gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp CODE32_SEG:pm32_return

BITS 32
pm32_return:
    mov ax, DATA32_SEG
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, [saved_esp32]

    lidt [saved_idtr]
    popad
    ret

; ---------- поиск и установка VBE-режима ----------
BITS 16
vbe_find_and_set_mode:
    push es
    push di

    mov ax, VBE_INFO_SEG
    mov es, ax
    xor di, di
    mov dword [es:di], 'VBE2'
    mov ax, 0x4F00
    int 0x10
    cmp ax, 0x004F
    jne .fail

    mov ax, [es:di+14]
    mov bx, [es:di+16]
    mov [mode_list_off], ax
    mov [mode_list_seg], bx

.next_mode:
    mov es, [mode_list_seg]
    mov di, [mode_list_off]
    mov cx, [es:di]
    add word [mode_list_off], 2
    cmp cx, 0xFFFF
    je .fail

    push cx
    mov ax, MODE_INFO_SEG
    mov es, ax
    xor di, di
    pop cx
    push cx
    mov ax, 0x4F01
    int 0x10
    cmp ax, 0x004F
    pop cx
    jne .next_mode

    mov ax, [es:di+18]
    cmp ax, TARGET_WIDTH
    jne .next_mode
    mov ax, [es:di+20]
    cmp ax, TARGET_HEIGHT
    jne .next_mode
    mov al, [es:di+25]
    cmp al, TARGET_BPP
    jne .next_mode

    mov [found_mode], cx
    mov eax, [es:di+40]
    mov [fb_addr], eax
    mov ax, [es:di+16]
    mov [fb_pitch], ax

    mov bx, [found_mode]
    or bx, 0x4000
    mov ax, 0x4F02
    int 0x10
    cmp ax, 0x004F
    jne .fail

    xor ax, ax
    mov es, ax
    mov di, FB_INFO_ADDR
    mov eax, [fb_addr]
    mov [es:di], eax
    mov ax, [fb_pitch]
    mov [es:di+4], ax
    mov word [es:di+6], TARGET_WIDTH
    mov word [es:di+8], TARGET_HEIGHT
    mov byte [es:di+10], TARGET_BPP

    pop di
    pop es
    ret

.fail:
    xor ax, ax
    mov es, ax
    mov di, FB_INFO_ADDR
    mov dword [es:di], 0
    pop di
    pop es
    ret

; ---------- данные ----------
BITS 32
saved_idtr:  dw 0
             dd 0
saved_esp32: dd 0
real_idtr:   dw 0x3FF
             dd 0x00000000

mode_list_off: dw 0
mode_list_seg: dw 0
found_mode:    dw 0
fb_addr:       dd 0
fb_pitch:      dw 0

; ---------- собственный GDT трамплина (не зависит от boot.asm) ----------
local_gdt_start:
    dq 0x0000000000000000
local_gdt_code32:
    dw 0xFFFF, 0x0000
    db 0x00, 10011010b, 11001111b, 0x00
local_gdt_data32:
    dw 0xFFFF, 0x0000
    db 0x00, 10010010b, 11001111b, 0x00
local_gdt_code16:
    dw 0xFFFF, 0x0000
    db 0x00, 10011010b, 00001111b, 0x00
local_gdt_data16:
    dw 0xFFFF, 0x0000
    db 0x00, 10010010b, 00001111b, 0x00
local_gdt_end:
local_gdt_descriptor:
    dw local_gdt_end - local_gdt_start - 1
    dd local_gdt_start

CODE32_SEG equ local_gdt_code32 - local_gdt_start
DATA32_SEG equ local_gdt_data32 - local_gdt_start
CODE16_SEG equ local_gdt_code16 - local_gdt_start
DATA16_SEG equ local_gdt_data16 - local_gdt_start

VBE_INFO_SEG   equ 0x9000
MODE_INFO_SEG  equ 0x9200
TARGET_WIDTH   equ 1024
TARGET_HEIGHT  equ 768
TARGET_BPP     equ 32
FB_INFO_ADDR   equ 0x0500
