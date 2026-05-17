#ifndef ELF_H
#define ELF_H

#define ELF_MAGIC 0x464C457F

typedef struct {
    unsigned int  magic;
    unsigned char bits;
    unsigned char endian;
    unsigned char version;
    unsigned char os_abi;
    unsigned char padding[8];
    unsigned short type;
    unsigned short machine;
    unsigned int  version2;
    unsigned int  entry;
    unsigned int  phoff;
    unsigned int  shoff;
    unsigned int  flags;
    unsigned short ehsize;
    unsigned short phentsize;
    unsigned short phnum;
    unsigned short shentsize;
    unsigned short shnum;
    unsigned short shstrndx;
} __attribute__((packed)) elf_header_t;

typedef struct {
    unsigned int type;
    unsigned int offset;
    unsigned int vaddr;
    unsigned int paddr;
    unsigned int filesz;
    unsigned int memsz;
    unsigned int flags;
    unsigned int align;
} __attribute__((packed)) elf_phdr_t;

int elf_load(const char *name);

#endif