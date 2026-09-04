#include "lclib.h"
#include "lib/bare/bare.h"

int soundbool = 0;
int graphic_mode = 0;
extern unsigned char rm_video_blob[];
extern unsigned int  rm_video_blob_len;

static void video_init(void) {
    memcpy((void*)TRAMPOLINE_ADDR, rm_video_blob, rm_video_blob_len);
    void (*fn)(void) = (void (*)(void))TRAMPOLINE_ADDR;
    fn();
}

void kmain(void) {
    idt_install();
    pic_remap(0x20, 0x28);
    timer_init(100);
    keyboard_init();
    __asm__ volatile ("sti");
    char *video = (char *) 0xB8000;
    vga_clearscreen(0x0F, video);
    if (graphic_mode == 1) {
        video_init();
        put_pixel(512,384,0xF000F000);
    } else {
        int maxcol = vga_getmaxcol();
        int maxrow = vga_getmaxrow();

        vga_clearrow(0, 0x1F, video);
        vga_clearrow(1, 0x1F, video);
        vga_rputsat("lap", 0x1F, 0, 0, video);
        vga_rputsat("32bit", 0x1F, 0, maxcol-5, video);
        vga_drawhline("-", 0x1F, 1, 0, maxcol, video);
        vga_rputsat("cmd", 0x1F, 1, maxcol/2-2, video);
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
        posrow = 3;
        poscol = 2;
        char cbuf[256] = {0};
        int len = 0;
        vga_rputsat(">", 0x0F, posrow, 0, video);
        for (;;) {
            if (posrow == maxrow) {
                for (int i = 2; i <= maxrow; i++) {
                    vga_clearrow(i, 0x0F, video);
                }
                posrow = 3;
            }

            ensure_row_bounds();
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
                    if (poscol > 2 && poscol <= maxcol) {
                        poscol--;
                        cbuf[len--] = 0;
                    }
                    char buf[2] = {' ', '\0'};
                    vga_rputsat(buf, 0x0F, posrow, poscol, video);
                    continue;
                }

                if (c == '\n') {
                    posrow++;
                    poscol = 2;
                    cbuf[len] = '\0';
                    tryParseCommand(cbuf);
                    memset(cbuf, 0, sizeof(cbuf));
                    len = 0;
                    vga_rputsat(">", 0x0F, posrow, 0, video);
                    continue;
                }

                if (c == '\t') {
                    for (int i = 0; i != 2; i++) {
                        cbuf[len++] = ' ';
                        char buf[2] = {' ', '\0'};
                        vga_rputsat(buf, 0x0F, posrow, poscol, video);
                        poscol++;
                    }
                    continue;
                }
                cbuf[len++] = c;
                char buf[2] = {c, '\0'};
                vga_rputsat(buf, 0x0F, posrow, poscol, video);
                poscol++;
            }
        }

        return;
    }

}
