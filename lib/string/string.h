#ifndef STRING_H
#define STRING_H

#include <stddef.h>

size_t strlen(const char *s);
void  *memcpy(void *dst, const void *src, size_t n);
void  *memset(void *dst, int c, size_t n);
int    memcmp(const void *s1, const void *s2, size_t n);
int    strcmp(const char *s1, const char *s2);
int    atoi(const char *s);
int    hextoi(const char *s);

#endif
