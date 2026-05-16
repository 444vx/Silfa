#ifndef SFS_H
#define SFS_H

#define SFS_MAGIC      0x53464653
#define SFS_MAX_FILES  16
#define SFS_NAME_LEN   32
#define SFS_DATA_START 64

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

void sfs_init(void);
int  sfs_create(const char *name);
int  sfs_write(const char *name, const char *data, unsigned int size);
int  sfs_read(const char *name, char *buf, unsigned int size);
int  sfs_delete(const char *name);
int  sfs_getname(int index, char *out);

#endif