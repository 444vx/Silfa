#include "keyboard.h"

static inline unsigned char inb(unsigned short port) {
    unsigned char val;
    __asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

static inline void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static void io_wait(void) {
    outb(0x80, 0x00);
}

static const char scancode_table[128] = {
    0,    0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
    '\b', '\t','q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
    '\n', 0,   'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';','\'', '`',
    0,  '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,   '*',
    0,   ' ',  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,    0,   0,   0,   '-', 0,   0,   0,   '+', 0,   0,   0,   0,
    0,   0,    0,   0,   0,   0,   0,   0
};

static unsigned short *vga = (unsigned short *)0xB8000;
static int cur_x = 0;
static int cur_y = 14;

static void vga_putchar(char c) {
    if (c == '\n') {
        cur_x = 0;
        cur_y++;
    } else if (c == '\b') {
        if (cur_x > 0) {
            cur_x--;
            vga[cur_y * 80 + cur_x] = (0x07 << 8) | ' ';
        }
    } else {
        vga[cur_y * 80 + cur_x] = (0x07 << 8) | (unsigned char)c;
        cur_x++;
        if (cur_x >= 80) {
            cur_x = 0;
            cur_y++;
        }
    }
}

void keyboard_handler(void) {
    unsigned char scancode = inb(0x60);
    outb(0x20, 0x20);
    if (scancode & 0x80) {
        return;
    }
    if (scancode < 128) {
        char c = scancode_table[scancode];
        if (c) {
            vga_putchar(c);
        }
    }
}

void keyboard_init(void) {
    outb(0x20, 0x11); io_wait();
    outb(0xA0, 0x11); io_wait();
    outb(0x21, 0x20); io_wait();
    outb(0xA1, 0x28); io_wait();
    outb(0x21, 0x04); io_wait();
    outb(0xA1, 0x02); io_wait();
    outb(0x21, 0x01); io_wait();
    outb(0xA1, 0x01); io_wait();
    outb(0x21, 0xFD); io_wait();
    outb(0xA1, 0xFF); io_wait();
}