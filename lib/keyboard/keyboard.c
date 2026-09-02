#include "keyboard.h"
#include "../pic/pic.h"
#include "../idt/idt.h"
#include "../bare/bare.h"

static volatile int kb_buffer[256];
static volatile int kb_head = 0;
static volatile int kb_tail = 0;

#define SC_LSHIFT 0x2A
#define SC_RSHIFT 0x36
#define SC_CAPSLOCK 0x3A

static volatile int shift_pressed = 0;
static volatile int caps_lock_on = 0;

#define DATA_PORT 0x60
#define SC_RELEASE 0x80

extern void irq1_stub(void);

void keyboard_handler(void) {
    int scancode = inb(DATA_PORT);
    uint8_t code = scancode_to_keycode((uint8_t)scancode);
    int released = is_key_release((uint8_t)scancode);

    if (code == SC_LSHIFT || code == SC_RSHIFT) {
        shift_pressed = !released;
    } else if (code == SC_CAPSLOCK && !released) {
        caps_lock_on = !caps_lock_on;
    }

    int next = (kb_head + 1) % 256;
    if (next != kb_tail) {
        kb_buffer[kb_head] = scancode;
        kb_head = next;
    }
    pic_send_eoi(1);
}

int keyboard_has_scancode(void) {
    return kb_head != kb_tail;
}

int keyboard_read_scancode(void) {
    while (!keyboard_has_scancode()) {
        asm volatile ("hlt");
    }
    int sc = kb_buffer[kb_tail];
    kb_tail = (kb_tail + 1) % 256;
    return sc;
}

void keyboard_init(void) {
    idt_set_gate(0x21, (uint32_t)irq1_stub, 0x08, 0x8E);
    pic_unmask_irq(1);
}

int is_key_release(uint8_t sc) {
    return (sc & SC_RELEASE) != 0;
}
int scancode_to_keycode(uint8_t sc) {
    return sc & ~SC_RELEASE;
}

static const char scancode_ascii[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i','o','p','[',']','\n',0,'a','s','d','f','g','h','j','k','l',';','\'', '`', 0, '\\', 'z','x','c','v','b','n','m',',','.','/', 0
, '*', 0, ' '};

static const char scancode_ascii_shifted[] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t',
    'Q','W','E','R','T','Y','U','I','O','P','{','}', '\n', '0',
    'A','S','D','F','G','H','J','K','L',':','"', '~', 0, '|',
    'Z','X','C','V','B','N','M','<','>','?', 0,
    '*', 0, ' '
};

static int is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

char keycode_to_ascii(uint8_t keycode) {
    if (keycode == SC_CAPSLOCK || keycode == SC_LSHIFT || keycode == SC_RSHIFT) {
        return 0;
    }

    if (keycode >= sizeof(scancode_ascii)) {
        return 0;
    }

    char base = scancode_ascii[keycode];
    char shifted = scancode_ascii_shifted[keycode];

    int use_shift = shift_pressed;
    if (is_letter(base) && caps_lock_on) {
        use_shift = !use_shift;
    }

    return use_shift ? shifted : base;
}
