#include "idt.h"

void isr_handler(void)
{
    unsigned char *vga = (unsigned char *)0xB8000 + (80 * 12 * 2);
    const char *msg = "Silfa: interrupt!";
    int i = 0;

    while (msg[i] != 0)
    {
        vga[i * 2] = msg[i];
        vga[i * 2 + 1] = 0x0F;
        i++;
    }
}

void kernel_main(void)
{
    unsigned char *vga = (unsigned char *)0xB8000 + (80 * 10 * 2);
    const char *msg = "Silfa 0.1";
    int i = 0;

    while (msg[i] != 0)
    {
        vga[i * 2] = msg[i];
        vga[i * 2 + 1] = 0x0F;
        i++;
    }

    idt_init();

    while (1)
    {
        __asm__("hlt");
    }
}