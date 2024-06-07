ORG 0x7C00
BITS 16

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

_start:
    jmp short start
    nop
times 33 db 0

start:
    jmp 0x0:step2

step2:
    cli ; disable interrupts
    mov ax, 0x0
    ;set up segments registers
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti ; enable interrupts

;load GDT
.load_protected:
    cli
    lgdt [gdt_descriptor]
    mov  eax, cr0
    or   eax, 1
    mov  cr0, eax
    jmp CODE_SEG:load32_protected

;GDT
gdt_start:
gdt_null:
    dd 0
    dd 0

;offset 0x08
gdt_code:     ; CS should point to this
    dw 0xFFFF ; segment limit 0-15 bits
    dw 0      ; segment base 0-15 bits
    db 0      ; segment base 16-23 bits
    db 0x9A   ; default access rights
    db 0xCF   ; 11001111 - Highest 4 bits are flags, 4 bits are limit 16-19 bits
    db 0      ; segment base 24-31 bits

;offset 0x10
gdt_data:     ; DS, ,SS ,ES, FS, GS should point to this
    dw 0xFFFF ; segment limit 0-15 bits
    dw 0      ; segment base 0-15 bits
    db 0      ; segment base 16-23 bits
    db 0x92   ; default access rights
    db 0xCF   ; 11001111 - Highest 4 bits are flags, 4 bits are limit 16-19 bits
    db 0      ; segment base 24-31 bits

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

[BITS 32]
load32_protected:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov fs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp
    jmp $

times 510-($-$$) db 0
    dw 0xAA55
