#ifndef CONFIG_H
#define CONFIG_H


#define KERNEL_CODE_SELECTOR            0x08
#define KERNEL_DATA_SELECTOR            0x10
#define FASKA_OS_TOTAL_INTERRUPTS         512

// 100Mb is the size of the kernel heap
#define FASKA_OS_HEAP_SIZE_BYTES        104857600
#define FASKA_OS_HEAP_BLOCK_SIZE        4096
#define FASKA_OS_HEAP_START_ADDRESS     0x01000000
#define FASKA_OS_HEAP_TABLE_ADDRESS     0x00007E00

#define FASKA_OS_SECTOR_SIZE             512

#endif //CONFIG_H