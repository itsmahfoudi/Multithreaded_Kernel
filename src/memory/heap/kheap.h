#ifndef MULTITHREADED_KERNEL_KHEAP_H
#define MULTITHREADED_KERNEL_KHEAP_H
#include <stddef.h>
#include <stdint.h>

void kheap_init();
void* kmalloc(size_t size);
void kfree(void* ptr);
void* kzalloc(size_t size);
#endif //MULTITHREADED_KERNEL_KHEAP_H
