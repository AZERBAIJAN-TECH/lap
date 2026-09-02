#ifndef RAWGUI_H
#define RAWGUI_H

#include <stdint.h>
void vga_drawhline(char* symb, uint8_t color, int row, int colstart, int colstop, volatile char* video);
void vga_drawvline(char* symb, uint8_t color, int col, int rowstart, int rowstop, volatile char* video);
#endif
