#include <stdint.h>
#include "rawgui.h"
#include "../bare/bare.h"
int vga_drawhline(char* symb, uint8_t color, int row, int colstart, int colstop, volatile char* video) {
    for (int i = colstart; i < colstop; i++) {
        vga_rputsat(symb, color, row, i, video);
    }
    return 0;
}
int vga_drawvline(char* symb, uint8_t color, int col, int rowstart, int rowstop, volatile char* video) {
    for (int i = rowstart; i < rowstop; i++) {
        vga_rputsat(symb, color, i, col, video);
    }
    return 0;
}
