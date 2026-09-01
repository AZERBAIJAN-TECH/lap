#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
void keyboard_handler(void);
int keyboard_has_scancode(void);
int keyboard_read_scancode(void);
void keyboard_init(void);
int is_key_release(uint8_t sc);
int scancode_to_keycode(uint8_t sc);
char keycode_to_ascii(uint8_t keycode);
#endif
