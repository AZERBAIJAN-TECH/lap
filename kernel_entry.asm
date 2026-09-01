; kernel_entry.asm — первый файл в связке ядра, должен идти первым при линковке,
; чтобы оказаться по адресу KERNEL_OFFSET (0x1000)
; nasm -f elf32 kernel_entry.asm -o kernel_entry.o

BITS 32

extern kmain

global _start
_start:
    call kmain
    cli
.hang:
    hlt
    jmp .hang
