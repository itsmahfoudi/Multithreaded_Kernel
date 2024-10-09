#ifndef MULTITHREADED_KERNEL_DISK_H
#define MULTITHREADED_KERNEL_DISK_H

int disk_read_sector(int lba, int total, void* buffer);

#endif //MULTITHREADED_KERNEL_DISK_H
