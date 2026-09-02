#include "../../lib/bare/bare.h"
#include "../../lib/string/string.h"
#include "../../lib/rawgui/rawgui.h"
#include "../../lib/state/state.h"
int clear(int argv, char *argc[]) {
    char *video = (char *) 0xB8000;
    int maxcol = vga_getmaxcol();
    int maxrow = vga_getmaxrow();

    vga_clearscreen(0x0F, video);
    vga_clearrow(0, 0x1F, video);
    vga_clearrow(1, 0x1F, video);
    vga_rputsat("lap", 0x1F, 0, 0, video);
    vga_rputsat("32bit", 0x1F, 0, maxcol-5, video);
    vga_drawhline("-", 0x1F, 1, 0, maxcol/2-5, video);
    vga_drawhline("-", 0x1F, 1, maxcol/2+4, maxcol, video);
    vga_rputsat("text edit", 0x1F, 1, maxcol/2-5, video);
    vga_rputsat(">", 0x0F, 3, 0, video);
    posrow = 3;
    poscol = 2;
    return 0;
}
int clearrow(int argv, char *argc[]) {
    if (argv < 2) {
        return 1;
    }
    char *video = (char *) 0xB8000;
    int row = atoi(argc[0]);
    int color = hextoi(argc[1]);
    vga_clearrow(row, color, video);
    return 0;
}

int clearscreen(int argv, char *argc[]) {
    if (argv < 1) {
        return 1;
    }
    char *video = (char *) 0xB8000;
    vga_clearscreen(hextoi(argc[0]), video);
    return 0;
}
