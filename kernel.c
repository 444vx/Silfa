#include "idt.h"
#include "keyboard.h"

static unsigned short *vga = (unsigned short *)0xB8000;

static void vga_print(const char *msg, int row, unsigned char color) {
    int i = 0;
    while (msg[i]) {
        vga[row * 80 + i] = (color << 8) | (unsigned char)msg[i];
        i++;
    }
}

void isr_handler(void) {
    vga_print("Silfa: interrupt!", 12, 0x0F);
}

void kernel_main(void) {
    vga_print("Silfa 0.1", 10, 0x0F);
    vga_print("IDT init...", 11, 0x07);
    idt_init();
    vga_print("IDT OK", 11, 0x0F);
    vga_print("KB init...", 12, 0x07);
    keyboard_init();
    vga_print("KB OK", 12, 0x0F);
    __asm__ volatile ("sti");
    while (1) {
        __asm__("hlt");
    }
}