#include "sound.h"
#include <stdint.h>
#include "../bare/bare.h"
void sound(uint32_t freq) {
    uint32_t div = 1193180 / freq;
    outb(0x43, 0xb6);
    outb(0x42, (uint8_t)(div));
    outb(0x42, (uint8_t)(div >> 8));
    uint8_t tmp = inb(0x61);
    if (tmp != (tmp | 3)) {
        outb(0x61, tmp | 3);
    }
}
void sound_stop() {
    uint8_t tmp = inb(0x61) & 0xFC;
    outb(0x61, tmp);
}
