#include "string.h"

size_t strlen(const char *s) {
    const char *p = s;
    while (*p) p++;
    return (size_t)(p - s);
}

void *memcpy(void *dst, const void *src, size_t n) {
    unsigned char *d = dst;
    const unsigned char *s = src;
    while (n--) *d++ = *s++;
    return dst;
}

void *memset(void *dst, int c, size_t n) {
    unsigned char *d = dst;
    while (n--) *d++ = (unsigned char)c;
    return dst;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *a = s1, *b = s2;
    while (n--) {
        if (*a != *b) return *a - *b;
        a++; b++;
    }
    return 0;
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) { s1++; s2++; }
    return (unsigned char)*s1 - (unsigned char)*s2;
}

int atoi(const char *s) {
    int sign = 1;
    int result = 0;

    while (*s == ' ') s++;

        if (*s == '-') { sign = -1; s++; }
        else if (*s == '+') { s++; }

        while (*s >= '0' && *s <= '9') {
            result = result * 10 + (*s - '0');
            s++;
        }

        return result * sign;
}
int hextoi(const char *s) {
    int result = 0;
    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
        s += 2;
    }
    while (*s) {
        char c = *s;
        int digit;
        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else if (c >= 'a' && c <= 'f') {
            digit = c - 'a' + 10;
        } else if (c >= 'A' && c <= 'F') {
            digit = c - 'A' + 10;
        } else {
            break;
        }
        result = result * 16 + digit;
        s++;
    }
    return result;
}
