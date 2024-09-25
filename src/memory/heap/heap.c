#include "heap.h"
#include "kernel.h"
#include "status.h"
#include "./../memory.h"
static int heap_validate_alignment(void* ptr) {
    return ((unsigned int)ptr) % FASKA_OS_HEAP_BLOCK_SIZE == 0;
}

static int heap_validate_table(void* ptr_start, void* ptr_end, heap_table* table) {
    int result = 0;
    size_t table_size = ptr_end - ptr_start;
    size_t table_blocks = table_size / FASKA_OS_HEAP_BLOCK_SIZE;
    if (table_blocks != table->total) {
        result = -EINVARG;
        goto out;
    }
    out:
    return result;
}

static uint32_t heap_align_value_to_upper(uint32_t val) {
    if (val % FASKA_OS_HEAP_BLOCK_SIZE == 0) {
        return val;
    }
    val = ( val - ( val % FASKA_OS_HEAP_BLOCK_SIZE)) + FASKA_OS_HEAP_BLOCK_SIZE;
    return val;
}

int heap_create(heap* h, void *ptr_start, void* ptr_end, heap_table* table) {
    int res = 0;
    if (!heap_validate_alignment(ptr_start) || !heap_validate_alignment(ptr_end)) {
       res = -EINVARG;
       goto out;
    }
    memset(h, 0, sizeof(heap));
    h->start_address = ptr_start;
    h->table = table;

    res = heap_validate_table(ptr_start, ptr_end, table);
    if (res < 0) {
        goto out;
    }
    size_t table_size = table->total * sizeof(HEAP_BLOCK_TABLE_ENTRY);
    memset(table->entries, 0, table_size);
    out:
    return res;
}

int heap_get_start_block(heap* h, uint32_t total_blocks) {
    heap_table *table = h->table;
    int current_block = 0;
    int start_block = -1;
    for(size_t i = 0; i < table->total; i++) {
        if ((table->entries[i] & 0x0f) != HEAP_BLOCK_TABLE_ENTRY_FREE) {
            current_block =0;
            start_block = -1;
            continue;
        }

        // If this is the first block in the sequence
        if (start_block == -1) {
            start_block = i;
        }
        current_block++;
        if (current_block == total_blocks) {
            break;
        }
    }
    if (start_block == -1) {
        return -ENOMEM;
    }
    return start_block;
}

void* heap_block_to_address(heap* h, uint32_t block) {
    return (void*)(h->start_address + (block * FASKA_OS_HEAP_BLOCK_SIZE));
}

void heap_mark_blocks_taken(heap* h, uint32_t start_block, uint32_t total_blocks) {
    int end_block = start_block + total_blocks - 1;
    HEAP_BLOCK_TABLE_ENTRY entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN | HEAP_BLOCK_IS_FIRST;
    if(total_blocks > 1) {
        entry |= HEAP_BLOCK_HAS_NEXT;
    }
    for(size_t i = start_block; i <= end_block; i++) {
        h->table->entries[i] = entry;
        entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN;
        if (i != end_block - 1 ){
            entry |= HEAP_BLOCK_HAS_NEXT;
        }
    }
}

void* heap_malloc_blocks(heap* h, uint32_t total_blocks) {
    void* address = NULL;
    int start_block = heap_get_start_block(h, total_blocks);
    if (start_block < 0) {
        goto out;
    }
    address = (void*)heap_block_to_address(h, start_block);

    //Mark the blocks as taken
    heap_mark_blocks_taken(h, start_block, total_blocks);
    out:
    return address;
}

void* heap_alloc(heap *kernel_heap, size_t size) {
    size_t aligned_size = heap_align_value_to_upper(size);
    uint32_t total_blocks = aligned_size / FASKA_OS_HEAP_BLOCK_SIZE;
    return heap_malloc_blocks(kernel_heap, total_blocks);
}

int heap_address_to_block(heap* h, void* ptr) {
    return ((uint32_t)(ptr - h->start_address) / FASKA_OS_HEAP_BLOCK_SIZE);
}

void heap_mark_blocks_free(heap* h, uint32_t start_block) {
    heap_table* table = h->table;
    for (size_t i = start_block; i < table->total; i++) {
        HEAP_BLOCK_TABLE_ENTRY entry = table->entries[i];
        table->entries[i] = HEAP_BLOCK_TABLE_ENTRY_FREE;
        if ((entry & HEAP_BLOCK_HAS_NEXT) == 0) {
            break;
        }
    }
}

void heap_free(heap *h, void* ptr) {
    heap_mark_blocks_free(h, heap_address_to_block(h, ptr));
}
