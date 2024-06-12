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
    ;jmp $
    
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
    mov eax, 1 
    mov ecx, 100 
    mov edi, 0x100000
    call ata_lba_read
    jmp CODE_SEG:0x100000
     
ata_lba_read:
    mov ebx, eax ; Backup the LBA 
  ; Send the Highest 8 bits of the lba to the hard disk controller
    shr eax, 24
    or eax, 0xE0
    mov dx, 0x1F6
    out dx, al 
  ; Finished sending the Highest 8 to the lba 

  ;send to the total sectors to read 
    mov eax, ecx 
    mov dx, 0x1F2
    out dx, al 
  ; Finished sending the total sectors to read 
    
  ; send more bits to the lba 
    mov eax, ebx ; restore the Backup LBA 
    mov dx, 0x1F3
    out dx, al 
  ; Finished sending more bits to the lba
    
  ; sending more bits of the LBA
    mov dx, 0x1F4 
    mov eax, ebx ; restore the Backup LBA 
    shr eax, 8
    out dx, al 
  ; Finished sending more bits of the lba 
    
  ; send upper 16 bits of the LBA 
    mov dx, 0x1F5 
    mov eax, ebx ; restore the Backup LBA 
    shr eax, 16 
    out dx, al 
  ; Finished sending upper 16 bits of the lba

    mov dx, 0x1F7
    mov al, 0x20 
    out dx, al 
    
  ; Read all sectors into memory 
.next_sector:
    push ecx 
  
  ; checking if we need to read 
.try_again:
    mov dx, 0x1F7
    in al, dx 
    test al, 8
    jz .try_again

  ; we need to read 256 word at a time 
    mov ecx, 256 
    mov dx, 0x1F0
    rep insw
    pop ecx 
    loop .next_sector
  ; End of reading sectors
    ret   

times 510-($-$$) db 0
    dw 0xAA55
