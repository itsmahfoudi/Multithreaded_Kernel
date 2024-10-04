#include "paging.h"
#include "./../heap/kheap.h"
static uint32_t * current_directory = 0;
void paging_load_directory(uint32_t* directory);

paging_4gb_chunk_t* paging_new_4gb(uint8_t flags) {
    paging_4gb_chunk_t* chunk = (paging_4gb_chunk_t*)kzalloc(sizeof(paging_4gb_chunk_t));
    uint32_t* directory = (uint32_t*)kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);
    uint32_t offset = 0;
    for (uint8_t i = 0; i < PAGING_TOTAL_ENTRIES_PER_TABLE; i++) {
        uint32_t* entry = (uint32_t*)kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);
        for (int j = 0; j < PAGING_TOTAL_ENTRIES_PER_TABLE; j++) {
            entry[j] = (offset + (PAGING_PAGE_SIZE * j)) | flags;
        }
        offset += PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE;
        directory[i] = (uint32_t)entry | flags;
    }
    chunk->directory_entry = directory;
    return chunk;
}

void paging_switch(uint32_t* directory) {
    current_directory = directory;
    paging_load_directory(directory);
}
