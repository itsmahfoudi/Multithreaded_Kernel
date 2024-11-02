#ifndef MULTITHREADED_KERNEL_DISK_H
#define MULTITHREADED_KERNEL_DISK_H

typedef unsigned int FASKAOS_DISK_TYPE;

#define FASKAOS_DISK_TYPE_REAL 0x0

typedef struct disk{
    FASKAOS_DISK_TYPE type;
    int sector_size;
}disk;

void disk_search_and_init(void);
disk *disk_get(int index);
int disk_read_block(disk* idisk, unsigned int lba, int total, void* buffer);
#endif //MULTITHREADED_KERNEL_DISK_H
