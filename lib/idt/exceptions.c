#include "idt.h"
#include "../bare/bare.h"

static const char *exception_names[32] = {
    "Divide by zero", "Debug", "NMI", "Breakpoint",
    "Overflow", "Bound range", "Invalid opcode", "Device not available",
    "Double fault", "Coprocessor overrun", "Invalid TSS", "Segment not present",
    "Stack fault", "GPF", "Page fault", "Reserved",
    "x87 FP", "Alignment check", "Machine check", "SIMD FP",
    "Virtualization", "Control protection", "Reserved", "Reserved",
    "Reserved", "Reserved", "Reserved", "Reserved",
    "Hypervisor injection", "VMM communication", "Security", "Reserved"
};

void exception_handler(regs_t *r) {
    char *video = (char *) 0xB8000;
    int maxrow = vga_getmaxrow();
    vga_rputsat("exception: ", 0x4F, maxrow-1, 00, video);
    vga_rputsat(exception_names[r->vector], 0x4F, maxrow-1, 11, video);
    __asm__ volatile ("cli");
    for (;;) {
        __asm__ volatile ("hlt");
    }
}
