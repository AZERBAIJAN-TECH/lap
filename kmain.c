#include "lclib.h"
#include "lib/bare/bare.h"
#include "lib/keyboard/keyboard.h"
int soundbool = 1;


void kmain(void) {
    idt_install();
    pic_remap(0x20, 0x28);
    timer_init(100);
    keyboard_init();
    __asm__ volatile ("sti");
    char *video = (char *) 0xB8000;
    int maxcol = 0;
    int maxrow = 0;
    vga_getmaxcol(&maxcol);
    vga_getmaxrow(&maxrow);
    vga_clearscreen(0x0F, video);
    vga_clearrow(0, 0x1F, video);
    vga_clearrow(1, 0x1F, video);
    vga_clearrow(2, 0x1F, video);
    vga_rputsat("initializing", 0x1F, 0, maxcol/2-6, video);
    vga_rputsat("lap", 0x1F, 0, 0, video);
    vga_rputsat("32bit", 0x1F, 0, maxcol-5, video);
    vga_drawhline("-", 0x1F, 1, 0, maxcol/2-5, video);
    vga_drawhline("-", 0x1F, 1, maxcol/2+4, maxcol, video);
    vga_rputsat("text edit", 0x1F, 1, maxcol/2-5, video);
    if (soundbool) {
        int hz = 100.0;
        for (int i = 0; i <= 4; i++) {
            sleep_ms(500);
            sound_stop();
            sound(hz);
            hz *= 2;
        }
        sound_stop();
    }
    int posrow = 3;
    int poscol = 0;
    char cbuf[256] = {0};
    int len = 0;
    vga_rputsat("            ", 0x1F, 0, maxcol/2-6, video);
    for (;;) {
        if (posrow == maxrow) {
            for (int i = 2; i <= maxrow; i++) {
                vga_clearrow(i, 0x0F, video);
            }
            posrow = 3;
        }

        if (poscol == maxcol) {
            posrow++;
            poscol = 0;
        }

        vga_set_cursor(posrow, poscol, maxcol);

        int sc = keyboard_read_scancode();
        if (is_key_release(sc)) {
            continue;
        }

        uint8_t keycode = scancode_to_keycode(sc);
        char c = keycode_to_ascii(keycode);

        if (c) {
            if (c == '\b') {
                poscol--;
                cbuf[len--] = 0;
                char buf[2] = {' ', '\0'};
                vga_rputsat(buf, 0x0F, posrow, poscol, video);
                continue;
            }

            if (c == '\n') {
                posrow++;
                poscol = 0;
                cbuf[len] = '\0';
                vga_clearrow(2, 0x1F, video);
                vga_rputsat(cbuf, 0x1F, 2, maxcol/2-(len/2), video);
                memset(cbuf, 0, sizeof(cbuf));
                len = 0;
                continue;
            }
            cbuf[len++] = c;
            char buf[2] = {c, '\0'};
            vga_rputsat(buf, 0x0F, posrow, poscol, video);
            poscol++;
        }
    }

    for (;;) {
        __asm__ volatile ("hlt");
    }
}
