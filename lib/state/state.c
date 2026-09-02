#include "state.h"
#include "../bare/bare.h"

int posrow;
int poscol;

void ensure_row_bounds(void) {
    char *video = (char *) 0xB8000;
    int maxrow = vga_getmaxrow();
    if (posrow >= maxrow) {
        for (int i = 2; i <= maxrow; i++) {
            vga_clearrow(i, 0x0F, video);
        }
        posrow = 3;
    }
}
