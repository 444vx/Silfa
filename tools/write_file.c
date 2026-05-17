#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SECTOR_SIZE    512
#define SUPERBLOCK_LBA 33
#define ENTRIES_LBA    34
#define DATA_LBA       64
#define SFS_MAGIC      0x53464653
#define SFS_MAX_FILES  16
#define SFS_NAME_LEN   32

typedef struct {
    unsigned int magic;
    unsigned int file_count;
    unsigned int reserved[6];
} sfs_superblock_t;

typedef struct {
    char         name[SFS_NAME_LEN];
    unsigned int size;
    unsigned int lba;
    unsigned int used;
} sfs_entry_t;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: write_file <image> <file>\n");
        return 1;
    }

    FILE *img = fopen(argv[1], "r+b");
    if (!img) { printf("Error: cannot open image\n"); return 1; }

    FILE *f = fopen(argv[2], "rb");
    if (!f) { printf("Error: cannot open file\n"); return 1; }

    fseek(f, 0, SEEK_END);
    unsigned int size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *data = malloc(size);
    fread(data, 1, size, f);
    fclose(f);

    unsigned char sbuf[SECTOR_SIZE];
    unsigned char ebuf[SECTOR_SIZE * 4];

    fseek(img, SUPERBLOCK_LBA * SECTOR_SIZE, SEEK_SET);
    fread(sbuf, 1, SECTOR_SIZE, img);
    sfs_superblock_t *sb = (sfs_superblock_t *)sbuf;

    if (sb->magic != SFS_MAGIC) {
        memset(sbuf, 0, SECTOR_SIZE);
        sb->magic = SFS_MAGIC;
        sb->file_count = 0;
        fseek(img, SUPERBLOCK_LBA * SECTOR_SIZE, SEEK_SET);
        fwrite(sbuf, 1, SECTOR_SIZE, img);
        memset(ebuf, 0, sizeof(ebuf));
        fseek(img, ENTRIES_LBA * SECTOR_SIZE, SEEK_SET);
        fwrite(ebuf, 1, sizeof(ebuf), img);
    }

    fseek(img, ENTRIES_LBA * SECTOR_SIZE, SEEK_SET);
    sfs_entry_t entries[SFS_MAX_FILES];
    fread(entries, 1, sizeof(entries), img);

    int slot = -1;
    const char *name = argv[2];
    const char *slash = strrchr(name, '/');
    if (slash) name = slash + 1;

    for (int i = 0; i < SFS_MAX_FILES; i++) {
        if (!entries[i].used) { slot = i; break; }
    }

    if (slot == -1) { printf("Error: no space\n"); return 1; }

    memset(&entries[slot], 0, sizeof(sfs_entry_t));
    strncpy(entries[slot].name, name, SFS_NAME_LEN - 1);
    entries[slot].lba  = DATA_LBA + slot * 8;
    entries[slot].size = size;
    entries[slot].used = 1;
    sb->file_count++;

    fseek(img, SUPERBLOCK_LBA * SECTOR_SIZE, SEEK_SET);
    fwrite(sbuf, 1, SECTOR_SIZE, img);

    fseek(img, ENTRIES_LBA * SECTOR_SIZE, SEEK_SET);
    fwrite(entries, 1, sizeof(entries), img);

    unsigned int lba = DATA_LBA + slot * 8;
    unsigned int written = 0;
    unsigned char buf[SECTOR_SIZE];
    while (written < size) {
        memset(buf, 0, SECTOR_SIZE);
        unsigned int chunk = size - written;
        if (chunk > SECTOR_SIZE) chunk = SECTOR_SIZE;
        memcpy(buf, data + written, chunk);
        fseek(img, lba * SECTOR_SIZE, SEEK_SET);
        fwrite(buf, 1, SECTOR_SIZE, img);
        lba++;
        written += chunk;
    }

    fclose(img);
    free(data);
    printf("Written: %s (%u bytes)\n", name, size);
    return 0;
}