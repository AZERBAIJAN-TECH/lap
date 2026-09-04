#ifndef FB_INFO_H
#define FB_INFO_H
#include <stdint.h>

typedef struct {
    uint32_t addr;
    uint16_t pitch;
    uint16_t width;
    uint16_t height;
    uint8_t  bpp;
} __attribute__((packed)) fb_info_t;

#define FB_INFO_ADDR      0x0500
#define TRAMPOLINE_ADDR   0x8000

extern fb_info_t *fb;

#endif
