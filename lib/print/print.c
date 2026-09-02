#include "print.h"
#include "../bare/bare.h"
#include "../state/state.h"

void print(const char *msg) {
    char *video = (char *) 0xB8000;
    ensure_row_bounds();
    vga_rputsat(msg, 0x0F, posrow, 0, video);
    posrow++;
    poscol = 2;
    ensure_row_bounds();
}
