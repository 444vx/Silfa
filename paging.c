#include "paging.h"

static unsigned int page_directory[1024] __attribute__((aligned(4096)));
static unsigned int page_table_0[1024] __attribute__((aligned(4096)));

void paging_init(void) {
    for (int i = 0; i < 1024; i++) {
        page_directory[i] = 0x00000002;
    }

    for (int i = 0; i < 1024; i++) {
        page_table_0[i] = (i * PAGE_SIZE) | PAGE_PRESENT | PAGE_WRITABLE;
    }

    page_directory[0] = (unsigned int)page_table_0 | PAGE_PRESENT | PAGE_WRITABLE;

    __asm__ volatile (
        "mov %0, %%cr3\n"
        "mov %%cr0, %%eax\n"
        "or $0x80000000, %%eax\n"
        "mov %%eax, %%cr0\n"
        : : "r"(page_directory) : "eax"
    );
}

void paging_map(unsigned int virt, unsigned int phys, unsigned int flags) {
    unsigned int dir = virt / (PAGE_SIZE * 1024);
    unsigned int tbl = (virt / PAGE_SIZE) % 1024;
    if (dir == 0) {
        page_table_0[tbl] = phys | flags | PAGE_PRESENT;
    }
}