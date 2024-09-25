[BITS 32]

global _start
global problem
extern kernel_main

CODE_SEG equ 0x08 
DATA_SEG equ 0x10

_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov fs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp
    ;Enable the A20 line
    in al, 0x92
    or al, 2
    out 0x92, al

    ;remap the master programmable interrupt controller
    mov al, 00010001b
    out 0x20, al     ;Tell the master PIC

    mov al, 0x20     ;Interrupt 0x20 is where master ISR should start
    out 0x21, al     ;Set the master PIC to start at 0x20

    mov al, 00000001b
    out 0x21, al
    ;End of remapping the master PIC
    sti      ;Enable interrupts
    call kernel_main
    jmp $

problem:
    int 0

times 512 - ($ - $$) db 0

