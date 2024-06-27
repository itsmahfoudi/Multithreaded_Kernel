#include "idt.h"
#include "config.h"
#include "memory/memory.h"
#include "kernel.h"

struct idt_desc idt_descriptors[FASKAOS_TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;

extern void idt_load(struct idtr_desc* idtr);

void idt_zero() {
    print("Division by zero exception\n");
}

void idt_set(int interrupt_num, void* address) {
    struct idt_desc* desc = &idt_descriptors[interrupt_num];
    desc->offset_1 = (uint32_t)address & 0xFFFF;
    desc->selector = KERNEL_CODE_SELECTOR;
    desc->zero = 0x00;
    desc->type_attr = 0xEE;
    desc->offset_2 = ((uint32_t)address >> 16) & 0xFFFF;
}

void idt_init() {
    memset(idt_descriptors, 0, sizeof(struct idt_desc)*FASKAOS_TOTAL_INTERRUPTS);
    idtr_descriptor.limit = sizeof(struct idt_desc)*FASKAOS_TOTAL_INTERRUPTS - 1;
    idtr_descriptor.base = (uint32_t)idt_descriptors;

    idt_set(0, idt_zero);
    idt_load(&idtr_descriptor);
}