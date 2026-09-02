#ifndef BARE_H
#define BARE_H
#include <stdint.h>
static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void io_wait(void) {
    asm volatile ("outb %%al, $0x80" : : "a"(0));
}
int vga_getmaxcol();
int vga_getmaxrow();
void vga_rputsat(const char *msg, uint8_t color, int row, int col, volatile char *video);
void vga_clearscreen(uint8_t color, volatile char *video);
void vga_clearrow(int row, uint8_t color, volatile char* video);
void vga_set_cursor(int row, int col, int maxcol);
#endif
