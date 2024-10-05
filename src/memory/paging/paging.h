#ifndef MULTITHREADED_KERNEL_PAGING_H
#define MULTITHREADED_KERNEL_PAGING_H

#include <stdint.h>
#include <stddef.h>

#define PAGING_CACHE_ENABLED            0b00010000
#define PAGING_WRITE_THROUGH            0b00001000
#define PAGING_ACCESS_FROM_ALL          0b00000100
#define PAGING_IS_WRITABLE              0b00000010
#define PAGING_IS_PRESENT               0b00000001


#define PAGING_TOTAL_ENTRIES_PER_TABLE  1024
#define PAGING_PAGE_SIZE                4096


typedef struct {
    uint32_t* directory_entry;
}paging_4gb_chunk_t;


paging_4gb_chunk_t* paging_new_4gb(uint8_t flags);
void paging_switch(uint32_t* directory);
void enable_paging();

uint32_t* paging_get_directory(paging_4gb_chunk_t* chunk);
uint8_t paging_is_aligned(void* address);
uint8_t paging_get_indexes(void* virtual_address, uint32_t* directory_index_out, uint32_t* table_index_out);
uint8_t paging_set(uint32_t* directory, void* virtual_address, uint32_t value);
#endif //MULTITHREADED_KERNEL_PAGING_H
