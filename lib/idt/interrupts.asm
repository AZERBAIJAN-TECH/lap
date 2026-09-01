BITS 32

global idt_load
global irq0_stub
global irq1_stub
extern timer_handler
extern keyboard_handler

idt_load:
    mov eax, [esp + 4]
    lidt [eax]
    ret

irq0_stub:
    pushad
    call timer_handler
    popad
    iretd
irq1_stub:
    pushad
    call keyboard_handler
    popad
    iretd
