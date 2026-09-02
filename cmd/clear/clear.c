#include "../../lib/bare/bare.h"
#include "../../lib/string/string.h"

int clear(int argv, char *argc[]) {
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
}
