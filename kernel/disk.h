#ifndef DISK_H
#define DISK_H

#define SECTOR_SIZE 512

int disk_read(unsigned int lba, unsigned char *buf);
int disk_write(unsigned int lba, unsigned char *buf);

#endif