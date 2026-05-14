void _kernel_main(void)
{
    char *vga = (char *)0xB8000;
    const char *msg = "Silfa Kernel v0.1";
    int i = 0;

    while (msg[i] != 0)
    {
        vga[i * 2] = msg[i];
        vga[i * 2 + 1] = 0x0F;
        i++;
    }

    while (1)
    {
        __asm__("hlt");
    }
}