#include "idt.h"
#include "config.h"
#include "memory/memory.h"
#include "kernel.h"
#include "io/io.h"

struct idt_desc idt_descriptors[FASKAOS_TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;

extern void int21h(void);
extern void idt_load(struct idtr_desc* idtr);
extern void no_interrupt(void);

void idt_zero() {
    print("Division by zero exception\n");
}

void int21h_handler(void) {
    print("Keyboard is pressed\n");
    outb(0x20, 0x20);
}

void no_interrupt_handler(void) {
    outb(0x20, 0x20);
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

    for (uint16_t i; i < FASKAOS_TOTAL_INTERRUPTS; i++) {
        idt_set(i, no_interrupt);
    }

    idt_set(0, idt_zero);
    idt_set(0x21, int21h);
    idt_load(&idtr_descriptor);
}