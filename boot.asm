; boot.asm — 16-bit boot sector (512 байт, BIOS grub-less загрузка)
; nasm -f bin boot.asm -o boot.bin

BITS 16
ORG 0x7C00

KERNEL_OFFSET equ 0x1000   ; куда грузим ядро в памяти
KERNEL_SECTORS equ 32      ; сколько секторов ядра читать с диска (подгони под размер)

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    mov [boot_drive], dl   ; BIOS передаёт номер диска в dl

    call load_kernel
    call enable_a20
    call load_gdt

    ; переход в protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp CODE_SEG:protected_mode_start

; ---------- загрузка ядра с диска (int 13h, extended read) ----------
load_kernel:
    mov si, dap
    mov ah, 0x42
    mov dl, [boot_drive]
    int 0x13
    jc disk_error
    ret

disk_error:
    mov si, disk_error_msg
    call print_string
    jmp $

print_string:
    mov ah, 0x0E
.next:
    lodsb
    or al, al
    jz .done
    int 0x10
    jmp .next
.done:
    ret

; Disk Address Packet для extended read (LBA)
dap:
    db 0x10          ; размер пакета
    db 0
    dw KERNEL_SECTORS
    dw KERNEL_OFFSET
    dw 0x0000        ; сегмент назначения
    dq 1             ; стартовый LBA (сектор 1, boot sector — это 0)

boot_drive: db 0
disk_error_msg: db "Disk read error", 0

; ---------- A20 line ----------
enable_a20:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

; ---------- GDT ----------
load_gdt:
    lgdt [gdt_descriptor]
    ret

gdt_start:
    dq 0x0000000000000000      ; null, 0x00
gdt_code32:
    dw 0xFFFF, 0x0000
    db 0x00, 10011010b, 11001111b, 0x00   ; 0x08
gdt_data32:
    dw 0xFFFF, 0x0000
    db 0x00, 10010010b, 11001111b, 0x00   ; 0x10
gdt_code16:
    dw 0xFFFF, 0x0000
    db 0x00, 10011010b, 00001111b, 0x00   ; 0x18
gdt_data16:
    dw 0xFFFF, 0x0000
    db 0x00, 10010010b, 00001111b, 0x00   ; 0x20
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG   equ gdt_code32 - gdt_start
DATA_SEG   equ gdt_data32 - gdt_start
CODE16_SEG equ gdt_code16 - gdt_start
DATA16_SEG equ gdt_data16 - gdt_start

; ---------- 32-bit protected mode ----------
BITS 32
protected_mode_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000   ; новый стек для 32-битного режима

    jmp KERNEL_OFFSET   ; переход на точку входа ядра (kernel_entry.asm)

; ---------- добивка до 510 байт + сигнатура загрузчика ----------
times 510 - ($ - $$) db 0
dw 0xAA55
