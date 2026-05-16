#include "disk.h"

static inline unsigned char inb(unsigned short port) {
    unsigned char val;
    __asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

static inline void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline unsigned short inw(unsigned short port) {
    unsigned short val;
    __asm__ volatile ("inw %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

static inline void outw(unsigned short port, unsigned short val) {
    __asm__ volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

static void ata_wait(void) {
    while (inb(0x1F7) & 0x80);
}

static void ata_ready(void) {
    while (!(inb(0x1F7) & 0x08));
}

int disk_read(unsigned int lba, unsigned char *buf) {
    ata_wait();
    outb(0x1F6, 0xE0 | ((lba >> 24) & 0x0F));
    outb(0x1F2, 1);
    outb(0x1F3, lba & 0xFF);
    outb(0x1F4, (lba >> 8) & 0xFF);
    outb(0x1F5, (lba >> 16) & 0xFF);
    outb(0x1F7, 0x20);
    ata_ready();

    unsigned short *ptr = (unsigned short *)buf;
    for (int i = 0; i < 256; i++) {
        ptr[i] = inw(0x1F0);
    }
    return 0;
}

int disk_write(unsigned int lba, unsigned char *buf) {
    ata_wait();
    outb(0x1F6, 0xE0 | ((lba >> 24) & 0x0F));
    outb(0x1F2, 1);
    outb(0x1F3, lba & 0xFF);
    outb(0x1F4, (lba >> 8) & 0xFF);
    outb(0x1F5, (lba >> 16) & 0xFF);
    outb(0x1F7, 0x30);
    ata_ready();

    unsigned short *ptr = (unsigned short *)buf;
    for (int i = 0; i < 256; i++) {
        outw(0x1F0, ptr[i]);
    }
    return 0;
}