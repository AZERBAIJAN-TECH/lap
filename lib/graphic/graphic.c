#include "graphic.h"
#include "../vid/fb_info.h"

void put_pixel(int x, int y, uint32_t color) {
    if ((uint32_t)x >= fb->width || (uint32_t)y >= fb->height) return;
    uint8_t *base = (uint8_t*)(uintptr_t)fb->addr;
    uint32_t offset = y * fb->pitch + x * 4;
    *(uint32_t*)(base + offset) = color;
}
