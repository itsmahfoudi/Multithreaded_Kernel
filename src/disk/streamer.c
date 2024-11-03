#include "streamer.h"
#include "memory/heap/kheap.h"
#include "config.h"

disk_stream *disk_streamer_create(int disk_id) {
    disk* new_disk = disk_get(disk_id);
    if (new_disk == NULL) {
        return NULL;
    }
    disk_stream* new_stream = (disk_stream*) kzalloc(sizeof(disk_stream));
    new_stream->disk = new_disk;
    new_stream->current_position = 0;
    return new_stream;
}

int disk_streamer_seek(disk_stream* stream, int offset) {
    stream->current_position = offset;
    return 0;
}

int disk_streamer_read(disk_stream* stream, void* out, int total) {
    int sector = stream->current_position / FASKA_OS_SECTOR_SIZE;
    int offset = stream->current_position % FASKA_OS_SECTOR_SIZE;
    char buffer[FASKA_OS_SECTOR_SIZE] = {0x00};

    int result = disk_read_block(stream->disk, sector, 1, buffer);
    if (result != 0) {
        goto out;
    }
    int total_to_read = total > FASKA_OS_SECTOR_SIZE ? FASKA_OS_SECTOR_SIZE : total;

    for (int i = 0; i < total_to_read; i++) {
        *(char*)out++ = buffer[offset+i];
    }

    // Adjust the stream
    stream->current_position += total_to_read;
    if (total > FASKA_OS_SECTOR_SIZE) {
        result = disk_streamer_read(stream, out, total - FASKA_OS_SECTOR_SIZE);
    }
    out:
    return result;
}

void disk_streamer_free(disk_stream* stream) {
    kfree(stream);
}