#ifndef MULTITHREADED_KERNEL_STREAMER_H
#define MULTITHREADED_KERNEL_STREAMER_H

#include "disk.h"

typedef struct disk_stream {
    unsigned int current_position;
    disk* disk;
} disk_stream;


disk_stream *disk_streamer_create(int disk_id);
int disk_streamer_seek(disk_stream* stream, int offset);
int disk_streamer_read(disk_stream* stream, void* out, int total);
void disk_streamer_free(disk_stream* stream);
#endif //MULTITHREADED_KERNEL_STREAMER_H
