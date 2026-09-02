#include "bare.h"
#include <stdint.h>
#define VGA_CTRL_PORT 0x3D4
#define VGA_DATA_PORT 0x3D5

int vga_getmaxcol() {
    volatile uint16_t *vgacol_addr = (volatile uint16_t*)0x44A;
    return *vgacol_addr;
}
int vga_getmaxrow() {
    volatile uint8_t *vgarow_addr = (volatile uint8_t*)0x484;
    return *vgarow_addr + 1;
}
void vga_rputsat(const char *msg, uint8_t color, int row, int col, volatile char *video) {
    int col_max = vga_getmaxcol();;
    int row_max = vga_getmaxrow();;
    int pos = row * col_max + col;
    for (int i = 0; msg[i] != '\0'; i++) {
        if (pos + i >= row_max * col_max) {
            break;
        }
        video[(pos + i) * 2] = msg[i];
        video[(pos + i) * 2 + 1] = color;
    }
    return;
}
void vga_clearscreen(uint8_t color, volatile char *video) {
    int col_max = vga_getmaxcol();
    int row_max = vga_getmaxrow();
    for (int i = 0; i != row_max; i++){
        for (int j = 0; j != col_max; j++) {
            int ofs = (i * col_max + j) * 2;
            video[ofs]     = ' ';
            video[ofs + 1] = color;
        }
    }
    return;
}

void vga_clearrow(int row, uint8_t color, volatile char* video) {
    int col_max = vga_getmaxcol();
    for (int i = 0; i != col_max; i++) {
        int ofs = (row * col_max + i) * 2;
        video[ofs]     = ' ';
        video[ofs + 1] = color;
    }
    return;
}

void vga_set_cursor(int row, int col, int maxcol) {
    uint16_t pos = row * maxcol + col;

    outb(VGA_CTRL_PORT, 0x0F);
    outb(VGA_DATA_PORT, (uint8_t)(pos & 0xFF));

    outb(VGA_CTRL_PORT, 0x0E);
    outb(VGA_DATA_PORT, (uint8_t)((pos >> 8) & 0xFF));
}
