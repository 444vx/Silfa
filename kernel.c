#include "idt.h"
#include "keyboard.h"
#include "mm.h"
#include "process.h"
#include "syscall.h"
#include "paging.h"
#include "tss.h"
#include "shell.h"
#include "disk.h"
#include "sfs.h"

static unsigned short *vga = (unsigned short *)0xB8000;
static unsigned int kernel_stack[1024];

static void vga_clear(void) {
    for (int i = 0; i < 80 * 25; i++) {
        vga[i] = (0x07 << 8) | ' ';
    }
}

static void vga_print(const char *msg, int row, unsigned char color) {
    int i = 0;
    while (msg[i]) {
        vga[row * 80 + i] = (color << 8) | (unsigned char)msg[i];
        i++;
    }
}

void isr_handler(void) {}
void process_yield(void) {}

void kernel_main(void) {
    vga_print("Booting Silfa OS...", 0, 0x07);
    idt_init();
    keyboard_init();
    mm_init();
    syscall_init();
    paging_init();
    tss_init((unsigned int)&kernel_stack[1023]);
    sfs_init();
    vga_clear();
    vga_print("Silfa Kernel 0.1", 0, 0x0F);
    vga_print("Type 'help' for commands", 1, 0x08);
    shell_init();
    __asm__ volatile ("sti");
    while (1) {
        __asm__("hlt");
    }
}