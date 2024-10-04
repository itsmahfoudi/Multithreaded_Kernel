#include "kheap.h"
#include "heap.h"
#include "config.h"
#include "kernel.h"
#include "./../memory.h"
heap kernel_heap;
heap_table kernel_heap_table;

void kheap_init() {
    int total_table_entries = FASKA_OS_HEAP_SIZE_BYTES / FASKA_OS_HEAP_BLOCK_SIZE;
    kernel_heap_table.entries = (HEAP_BLOCK_TABLE_ENTRY*)FASKA_OS_HEAP_TABLE_ADDRESS;
    kernel_heap_table.total = total_table_entries;

    int result = heap_create(&kernel_heap, (void*)FASKA_OS_HEAP_START_ADDRESS, (void*)(FASKA_OS_HEAP_START_ADDRESS + FASKA_OS_HEAP_SIZE_BYTES), &kernel_heap_table);
    if (result < 0) {
        print("Failed to create a heap\n");
    }
}

void* kmalloc(size_t size) {
    return heap_alloc(&kernel_heap, size);
}

void* kzalloc(size_t size) {
    void* ptr = kmalloc(size);
    if (!ptr) {
        return NULL;
    }
    memset(ptr, 0x00, size);
    return ptr;
}

void kfree(void* ptr) {
    heap_free(&kernel_heap, ptr);
}