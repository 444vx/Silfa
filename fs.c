#include "fs.h"

static file_t files[MAX_FILES];

static int strcmp(const char *a, const char *b) {
    while (*a && *b && *a == *b) { a++; b++; }
    return *a - *b;
}

static void strcpy(char *dst, const char *src) {
    while (*src) { *dst++ = *src++; }
    *dst = 0;
}

void fs_init(void) {
    for (int i = 0; i < MAX_FILES; i++) {
        files[i].used = 0;
        files[i].size = 0;
    }
}

int fs_create(const char *name) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].used && strcmp(files[i].name, name) == 0) return -1;
    }
    for (int i = 0; i < MAX_FILES; i++) {
        if (!files[i].used) {
            strcpy(files[i].name, name);
            files[i].size = 0;
            files[i].used = 1;
            return i;
        }
    }
    return -1;
}

int fs_write(const char *name, const char *data, unsigned int size) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].used && strcmp(files[i].name, name) == 0) {
            if (size > MAX_FILESIZE) size = MAX_FILESIZE;
            for (unsigned int j = 0; j < size; j++) {
                files[i].data[j] = data[j];
            }
            files[i].size = size;
            return 0;
        }
    }
    return -1;
}

int fs_read(const char *name, char *buf, unsigned int size) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].used && strcmp(files[i].name, name) == 0) {
            unsigned int n = files[i].size < size ? files[i].size : size;
            for (unsigned int j = 0; j < n; j++) {
                buf[j] = files[i].data[j];
            }
            return n;
        }
    }
    return -1;
}

int fs_delete(const char *name) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].used && strcmp(files[i].name, name) == 0) {
            files[i].used = 0;
            files[i].size = 0;
            return 0;
        }
    }
    return -1;
}

void fs_list(void) {
    unsigned short *vga = (unsigned short *)0xB8000;
    int row = 0;
    int col = 0;

    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].used) {
            col = 0;
            const char *n = files[i].name;
            while (*n) {
                vga[row * 80 + col] = (0x07 << 8) | (unsigned char)*n;
                col++;
                n++;
            }
            row++;
        }
    }
}

int fs_getname(int index, char *out) {
    if (index < 0 || index >= MAX_FILES) return 0;
    if (!files[index].used) return 0;
    int i = 0;
    while (files[index].name[i]) {
        out[i] = files[index].name[i];
        i++;
    }
    out[i] = 0;
    return 1;
}