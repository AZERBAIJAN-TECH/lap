#include "malloc.h"
#include <stdint.h>

#define HEAP_START 0x100000
#define HEAP_SIZE  (1 * 1024 * 1024)

typedef struct block_header {
    size_t size;
    struct block_header *next;
    int free;
} block_header_t;

#define ALIGN 8
#define ALIGN_UP(x) (((x) + (ALIGN - 1)) & ~((size_t)ALIGN - 1))
#define HEADER_SIZE ALIGN_UP(sizeof(block_header_t))

static block_header_t *free_list = NULL;
static uint8_t *heap_ptr = (uint8_t*)HEAP_START;
static uint8_t *heap_end = (uint8_t*)(HEAP_START + HEAP_SIZE);

static block_header_t *request_space(size_t size) {
    size_t total = HEADER_SIZE + size;
    if (heap_ptr + total > heap_end) {
        return NULL;
    }
    block_header_t *block = (block_header_t*)heap_ptr;
    heap_ptr += total;
    block->size = size;
    block->next = NULL;
    block->free = 0;
    return block;
}

static block_header_t *find_free_block(block_header_t **last, size_t size) {
    block_header_t *current = free_list;
    while (current && !(current->free && current->size >= size)) {
        *last = current;
        current = current->next;
    }
    return current;
}

void *malloc(size_t size) {
    if (size == 0) return NULL;
    size = ALIGN_UP(size);

    block_header_t *block;
    if (!free_list) {
        block = request_space(size);
        if (!block) return NULL;
        free_list = block;
    } else {
        block_header_t *last = free_list;
        block = find_free_block(&last, size);
        if (!block) {
            block = request_space(size);
            if (!block) return NULL;
            last->next = block;
        } else {
            block->free = 0;
        }
    }
    return (void*)(block + 1);
}

void free(void *ptr) {
    if (!ptr) return;
    block_header_t *block = (block_header_t*)ptr - 1;
    block->free = 1;
}
