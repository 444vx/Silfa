#include "syscall.h"
#include "mm.h"

static unsigned short *vga = (unsigned short *)0xB8000;
static int syscall_row = 17;

static void vga_print(const char *msg, int row, unsigned char color) {
    int i = 0;
    while (msg[i]) {
        vga[row * 80 + i] = (color << 8) | (unsigned char)msg[i];
        i++;
    }
}

int syscall_handler(int num, int arg1, int arg2, int arg3) {
    switch (num) {
        case SYSCALL_PRINT:
            vga_print((const char *)arg1, syscall_row++, 0x0F);
            return 0;
        case SYSCALL_EXIT:
            vga_print("Process exited", syscall_row++, 0x0C);
            return 0;
        case SYSCALL_MALLOC:
            return (int)kmalloc((unsigned int)arg1);
        case SYSCALL_FREE:
            kfree((void *)arg1);
            return 0;
        default:
            return -1;
    }
}

void syscall_init(void) {
    vga_print("Syscall OK", 16, 0x0F);
}