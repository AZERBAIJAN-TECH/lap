#ifndef IDT_H
#define IDT_H

#include <stdint.h>

void idt_set_gate(int n, uint32_t handler, uint16_t sel, uint8_t flags);
void idt_install(void);

#endif
