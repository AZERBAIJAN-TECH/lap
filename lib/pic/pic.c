#include "pic.h"
#include "../bare/bare.h"
#define PIC1        0x20
#define PIC2        0xA0
#define PIC1_DATA   0x21
#define PIC2_DATA   0xA1

#define ICW1_INIT   0x11
#define ICW4_8086   0x01


void pic_remap(int offset1, int offset2) {
    uint8_t mask1 = inb(PIC1_DATA);
    uint8_t mask2 = inb(PIC2_DATA);

    outb(PIC1, ICW1_INIT);
    outb(PIC2, ICW1_INIT);

    outb(PIC1_DATA, (uint8_t)offset1);
    outb(PIC2_DATA, (uint8_t)offset2);

    outb(PIC1_DATA, 4);
    outb(PIC2_DATA, 2);

    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    outb(PIC1_DATA, mask1);
    outb(PIC2_DATA, mask2);
}

void pic_send_eoi(uint8_t irq) {
    if (irq >= 8) {
        outb(PIC2, 0x20);
    }
    outb(PIC1, 0x20);
}

void pic_unmask_irq(uint8_t irq) {
    uint16_t port;
    uint8_t irq_line;

    if (irq < 8) {
        port = PIC1_DATA;
        irq_line = irq;
    } else {
        port = PIC2_DATA;
        irq_line = irq - 8;
    }

    uint8_t mask = inb(port);
    outb(port, mask & ~(1 << irq_line));
}
