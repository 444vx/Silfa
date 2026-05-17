void _start(void) {
    volatile unsigned short *vga = (volatile unsigned short *)0xB8000;
    vga[80 * 22 + 0] = (0x0A << 8) | 'H';
    vga[80 * 22 + 1] = (0x0A << 8) | 'I';
    vga[80 * 22 + 2] = (0x0A << 8) | '!';
}
