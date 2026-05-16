#ifndef MM_H
#define MM_H

void mm_init(void);
void *kmalloc(unsigned int size);
void kfree(void *ptr);

#endif