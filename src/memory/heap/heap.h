#ifndef HEAP_H
#define HEAP_H
#include "config.h"
#include <stdint.h>
#include <stddef.h>

#define HEAP_BLOCK_TABLE_ENTRY_TAKEN    0x01
#define HEAP_BLOCK_TABLE_ENTRY_FREE     0x00

#define HEAP_BLOCK_HAS_NEXT         0b10000000
#define HEAP_BLOCK_IS_FIRST         0b01000000

typedef unsigned char HEAP_BLOCK_TABLE_ENTRY;

typedef struct {
    HEAP_BLOCK_TABLE_ENTRY *entries;
    size_t total;
}heap_table;

typedef struct {
    heap_table *table;
    void* start_address;
}heap;

int heap_create(heap* h, void *ptr_start, void* ptr_end, heap_table* table);
void* heap_alloc(heap *h, size_t size);
void heap_free(heap *h, void* ptr);
void* heap_block_to_address(heap* h, uint32_t block);
int heap_get_start_block(heap* h, uint32_t total_blocks);
void heap_mark_blocks_taken(heap* h, uint32_t start_block, uint32_t total_blocks);
void* heap_malloc_blocks(heap* h, uint32_t total_blocks);
int heap_address_to_block(heap* h, void* ptr);
void heap_mark_blocks_free(heap* h, uint32_t start_block);
#endif //HEAP_H
