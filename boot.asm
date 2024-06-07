ORG 0x0
BITS 16

_start:
    jmp short start
    nop
times 33 db 0

start:
    jmp 0x7C0:step2

step2:
    cli ; disable interrupts
    mov ax, 0x07C0
    mov ds, ax
    mov es, ax
    mov ax, 0x00
    mov ss, ax
    mov sp, 0x7C00
    sti ; enable interrupts

    mov ah, 2 ; read sector command
    mov al, 1 ; read 1 sector
    mov ch, 0 ; cylinder 0 low eight bits
    mov cl, 2 ; read sector 2
    mov dh, 0 ; head number
    mov bx, buffer ; buffer address
    int 0x13 ; call BIOS
    jc error ; jump if carry flag is set
    ; if no error, print the buffer
    mov si, buffer
    call print
    jmp $

error:
    mov si, error_message
    call print
    jmp $

print:
    mov bx, 0
.loop:
    lodsb
    cmp al, 0
    je .done
    call print_char
    jmp .loop
.done:
    ret

print_char:
    mov ah, 0eh
    int 0x10
    ret

error_message:
    db 'Failed to load sector'

times 510-($-$$) db 0
    dw 0xAA55

buffer: