#ifndef PAGING_H
#define PAGING_H

#define PAGE_SIZE       4096
#define PAGE_PRESENT    0x1
#define PAGE_WRITABLE   0x2
#define PAGE_USER       0x4

void paging_init(void);
void paging_map(unsigned int virt, unsigned int phys, unsigned int flags);

#endif