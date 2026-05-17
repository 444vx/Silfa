#include "elf.h"
#include "sfs.h"
#include "mm.h"

#define PT_LOAD 1

static unsigned short *vga = (unsigned short *)0xB8000;

static void dbg(const char *msg, int row) {
    int i = 0;
    while (msg[i]) {
        vga[row * 80 + i] = (0x0C << 8) | (unsigned char)msg[i];
        i++;
    }
}

static void memcpy(void *dst, const void *src, unsigned int size) {
    unsigned char *d = (unsigned char *)dst;
    const unsigned char *s = (const unsigned char *)src;
    for (unsigned int i = 0; i < size; i++) d[i] = s[i];
}

static void memset(void *ptr, unsigned char val, unsigned int size) {
    unsigned char *p = (unsigned char *)ptr;
    for (unsigned int i = 0; i < size; i++) p[i] = val;
}

static void wait(void) {
    volatile unsigned int i = 0;
    while (i < 200000000) i++;
}

int elf_load(const char *name) {
    dbg("1:loading...", 20);
    wait();

    char *buf = (char *)kmalloc(32768);
    if (!buf) { dbg("2:nomem", 20); wait(); return -3; }

    dbg("2:kmalloc ok", 20);
    wait();

    int n = sfs_read(name, buf, 32767);
    if (n < 0) { dbg("3:notfound  ", 20); wait(); kfree(buf); return -1; }

    dbg("3:read ok   ", 20);
    wait();

    elf_header_t *hdr = (elf_header_t *)buf;
    if (hdr->magic != ELF_MAGIC) { dbg("4:badmagic  ", 20); wait(); kfree(buf); return -2; }

    dbg("4:magic ok  ", 20);
    wait();

    for (int i = 0; i < hdr->phnum; i++) {
        elf_phdr_t *ph = (elf_phdr_t *)(buf + hdr->phoff + i * hdr->phentsize);
        if (ph->type != PT_LOAD) continue;

        // pokaż adres segmentu
        unsigned int addr = ph->vaddr;
        char tmp[20];
        tmp[0] = '0'; tmp[1] = 'x';
        for (int j = 9; j >= 2; j--) {
            int nibble = addr & 0xF;
            tmp[j] = nibble < 10 ? '0' + nibble : 'A' + nibble - 10;
            addr >>= 4;
        }
        tmp[10] = 0;
        dbg(tmp, 21);
        wait();

        void *dst = (void *)ph->vaddr;
        memcpy(dst, buf + ph->offset, ph->filesz);
        if (ph->memsz > ph->filesz) {
            memset((unsigned char *)dst + ph->filesz, 0, ph->memsz - ph->filesz);
        }
    }

    dbg("5:mapped ok ", 20);
    wait();

    unsigned int entry = hdr->entry;
    kfree(buf);

    dbg("6:jumping...", 20);
    wait();

    void (*fn)(void) = (void (*)(void))entry;
    fn();

    dbg("7:returned  ", 20);
    wait();

    return 0;
}