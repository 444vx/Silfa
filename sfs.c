#include "sfs.h"
#include "disk.h"

#define SUPERBLOCK_LBA  33
#define ENTRIES_LBA     34
#define DATA_LBA        64

static sfs_superblock_t superblock;
static sfs_entry_t      entries[SFS_MAX_FILES];

static void memset(void *ptr, unsigned char val, unsigned int size) {
    unsigned char *p = (unsigned char *)ptr;
    for (unsigned int i = 0; i < size; i++) p[i] = val;
}

static void memcpy(void *dst, const void *src, unsigned int size) {
    unsigned char *d = (unsigned char *)dst;
    const unsigned char *s = (const unsigned char *)src;
    for (unsigned int i = 0; i < size; i++) d[i] = s[i];
}

static int strcmp(const char *a, const char *b) {
    while (*a && *b && *a == *b) { a++; b++; }
    return *a - *b;
}

static void strcpy(char *dst, const char *src) {
    while (*src) *dst++ = *src++;
    *dst = 0;
}

static void sfs_save_superblock(void) {
    unsigned char buf[512];
    memset(buf, 0, 512);
    memcpy(buf, &superblock, sizeof(sfs_superblock_t));
    disk_write(SUPERBLOCK_LBA, buf);
}

static void sfs_save_entries(void) {
    unsigned char buf[512];
    memset(buf, 0, 512);
    memcpy(buf, entries, sizeof(entries));
    disk_write(ENTRIES_LBA, buf);
}

static void sfs_load(void) {
    unsigned char buf[512];
    disk_read(SUPERBLOCK_LBA, buf);
    memcpy(&superblock, buf, sizeof(sfs_superblock_t));
    disk_read(ENTRIES_LBA, buf);
    memcpy(entries, buf, sizeof(entries));
}

void sfs_init(void) {
    sfs_load();
    if (superblock.magic != SFS_MAGIC) {
        memset(&superblock, 0, sizeof(sfs_superblock_t));
        superblock.magic      = SFS_MAGIC;
        superblock.file_count = 0;
        memset(entries, 0, sizeof(entries));
        sfs_save_superblock();
        sfs_save_entries();
    }
}

int sfs_create(const char *name) {
    for (int i = 0; i < SFS_MAX_FILES; i++) {
        if (entries[i].used && strcmp(entries[i].name, name) == 0) return -1;
    }
    for (int i = 0; i < SFS_MAX_FILES; i++) {
        if (!entries[i].used) {
            memset(&entries[i], 0, sizeof(sfs_entry_t));
            strcpy(entries[i].name, name);
            entries[i].lba  = DATA_LBA + i * 8;
            entries[i].size = 0;
            entries[i].used = 1;
            superblock.file_count++;
            sfs_save_superblock();
            sfs_save_entries();
            return i;
        }
    }
    return -1;
}

int sfs_write(const char *name, const char *data, unsigned int size) {
    for (int i = 0; i < SFS_MAX_FILES; i++) {
        if (entries[i].used && strcmp(entries[i].name, name) == 0) {
            if (size > 4096) size = 4096;
            unsigned char buf[512];
            unsigned int sectors = (size + 511) / 512;
            for (unsigned int s = 0; s < sectors; s++) {
                memset(buf, 0, 512);
                unsigned int offset = s * 512;
                unsigned int chunk  = size - offset;
                if (chunk > 512) chunk = 512;
                memcpy(buf, data + offset, chunk);
                disk_write(entries[i].lba + s, buf);
            }
            entries[i].size = size;
            sfs_save_entries();
            return 0;
        }
    }
    return -1;
}

int sfs_read(const char *name, char *buf, unsigned int size) {
    for (int i = 0; i < SFS_MAX_FILES; i++) {
        if (entries[i].used && strcmp(entries[i].name, name) == 0) {
            unsigned int n = entries[i].size < size ? entries[i].size : size;
            unsigned char sector[512];
            unsigned int sectors = (n + 511) / 512;
            for (unsigned int s = 0; s < sectors; s++) {
                disk_read(entries[i].lba + s, sector);
                unsigned int offset = s * 512;
                unsigned int chunk  = n - offset;
                if (chunk > 512) chunk = 512;
                memcpy(buf + offset, sector, chunk);
            }
            return n;
        }
    }
    return -1;
}

int sfs_delete(const char *name) {
    for (int i = 0; i < SFS_MAX_FILES; i++) {
        if (entries[i].used && strcmp(entries[i].name, name) == 0) {
            entries[i].used = 0;
            superblock.file_count--;
            sfs_save_superblock();
            sfs_save_entries();
            return 0;
        }
    }
    return -1;
}

int sfs_getname(int index, char *out) {
    if (index < 0 || index >= SFS_MAX_FILES) return 0;
    if (!entries[index].used) return 0;
    int i = 0;
    while (entries[index].name[i]) {
        out[i] = entries[index].name[i];
        i++;
    }
    out[i] = 0;
    return 1;
}