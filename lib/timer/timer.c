#include "timer.h"
#include "../pic/pic.h"
#include "../idt/idt.h"

#define PIT_CHANNEL0 0x40
#define PIT_COMMAND  0x43
#define PIT_BASE_HZ  1193182u
#include "../bare/bare.h"
static volatile uint32_t ticks = 0;
static uint32_t timer_hz = 100;

extern void irq0_stub(void);

void timer_handler(void) {
    ticks++;
    pic_send_eoi(0);
}

void timer_init(uint32_t hz) {
    timer_hz = hz;
    uint32_t divisor = PIT_BASE_HZ / hz;

    outb(PIT_COMMAND, 0x36);
    outb(PIT_CHANNEL0, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL0, (uint8_t)((divisor >> 8) & 0xFF));

    idt_set_gate(0x20, (uint32_t)irq0_stub, 0x08, 0x8E);
    pic_unmask_irq(0);
}

uint32_t get_ticks(void) {
    return ticks;
}

void sleep_ms(uint32_t ms) {
    uint32_t ticks_needed = (ms * timer_hz) / 1000;
    uint32_t target = ticks + ticks_needed;
    while (ticks < target) {
        __asm__ volatile ("hlt");
    }
}
