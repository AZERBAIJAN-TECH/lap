#include "print.h"
#include "../bare/bare.h"
#include "../state/state.h"

void print(const char *msg) {
    char *video = (char *) 0xB8000;
    int maxrow = vga_getmaxrow();

    vga_rputsat(msg, 0x0F, posrow, 0, video);
    posrow++;

    if (posrow == maxrow) {
        int maxcol = vga_getmaxcol();
        for (int i = 2; i <= maxrow; i++) {
            vga_clearrow(i, 0x0F, video);
        }
        posrow = 3;
    }
    poscol = 2;
}
