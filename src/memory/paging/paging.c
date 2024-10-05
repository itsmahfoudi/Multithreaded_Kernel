#include "paging.h"
#include "./../heap/kheap.h"
#include "status.h"

static uint32_t * current_directory = 0;
void paging_load_directory(uint32_t* directory);

paging_4gb_chunk_t* paging_new_4gb(uint8_t flags) {
    paging_4gb_chunk_t* chunk = (paging_4gb_chunk_t*)kzalloc(sizeof(paging_4gb_chunk_t));
    uint32_t* directory = (uint32_t*)kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);
    uint32_t offset = 0;
    for (int i = 0; i < PAGING_TOTAL_ENTRIES_PER_TABLE; i++) {
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

uint32_t* paging_get_directory(paging_4gb_chunk_t* chunk) {
    return chunk->directory_entry;
}

uint8_t paging_is_aligned(void* address) {
    return ((uint32_t)address % PAGING_PAGE_SIZE) == 0;
}

uint8_t paging_get_indexes(void* virtual_address, uint32_t* directory_index_out, uint32_t* table_index_out) {
    uint8_t result = 0;
    if (!paging_is_aligned(virtual_address)) {
        result = -EINVARG;
        goto out;
    }
    *directory_index_out = ((uint32_t)virtual_address / (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE)) ;
    *table_index_out = ((uint32_t)virtual_address % (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE)) / PAGING_PAGE_SIZE;

    out:
    return result;
}

uint8_t paging_set(uint32_t* directory, void* virtual_address, uint32_t value) {
    if (!paging_is_aligned(virtual_address)) {
        return -EINVARG;
    }
    uint32_t directory_index = 0;
    uint32_t table_index = 0;
    if (paging_get_indexes(virtual_address, &directory_index, &table_index) < 0) {
        return -EINVARG;
    }
    uint32_t* table = (uint32_t*)(directory[directory_index] & 0xFFFFF000);
    //The value has to contain physical address and flags
    table[table_index] = value;
    return 0;
}