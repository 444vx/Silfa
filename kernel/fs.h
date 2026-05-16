#ifndef FS_H
#define FS_H

#define MAX_FILES     16
#define MAX_FILENAME  32
#define MAX_FILESIZE  4096

typedef struct {
    char name[MAX_FILENAME];
    char data[MAX_FILESIZE];
    unsigned int size;
    int used;
} file_t;

void fs_init(void);
int fs_create(const char *name);
int fs_write(const char *name, const char *data, unsigned int size);
int fs_read(const char *name, char *buf, unsigned int size);
int fs_delete(const char *name);
int fs_getname(int index, char *out);
void fs_list(void);

#endif