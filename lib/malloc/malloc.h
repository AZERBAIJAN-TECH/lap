#ifndef MALLOC_H
#define MALLOC_H

#include <stddef.h>

void *malloc(size_t size);
void *memset(void *dst, int c, size_t n);
void  free(void *ptr);

#endif
