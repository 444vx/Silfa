#include "mm.h"

#define HEAP_START 0x100000
#define HEAP_SIZE  0x100000

typedef struct block {
    unsigned int size;
    int free;
    struct block *next;
} block_t;

static block_t *heap = (block_t *)HEAP_START;

void mm_init(void) {
    heap->size = HEAP_SIZE - sizeof(block_t);
    heap->free = 1;
    heap->next = 0;
}

void *kmalloc(unsigned int size) {
    block_t *cur = heap;

    while (cur) {
        if (cur->free && cur->size >= size) {
            if (cur->size > size + sizeof(block_t)) {
                block_t *new = (block_t *)((unsigned char *)cur + sizeof(block_t) + size);
                new->size = cur->size - size - sizeof(block_t);
                new->free = 1;
                new->next = cur->next;
                cur->size = size;
                cur->next = new;
            }
            cur->free = 0;
            return (unsigned char *)cur + sizeof(block_t);
        }
        cur = cur->next;
    }

    return 0;
}

void kfree(void *ptr) {
    if (!ptr) return;

    block_t *cur = (block_t *)((unsigned char *)ptr - sizeof(block_t));
    cur->free = 1;

    cur = heap;
    while (cur && cur->next) {
        if (cur->free && cur->next->free) {
            cur->size += sizeof(block_t) + cur->next->size;
            cur->next = cur->next->next;
        } else {
            cur = cur->next;
        }
    }
}