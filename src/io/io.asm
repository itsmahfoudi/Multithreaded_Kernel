section .asm

global insb ; Makes the label 'insb' accessible from other files.
global insw ; Makes the label 'insw' accessible from other files.
global outb ; Makes the label 'outsb' accessible from other files.
global outw ; Makes the label 'outsw' accessible from other files.

; insb - Reads a byte from a specified port into AL register.
;
; Description:
;     This function reads a byte from the input port specified by the
;     port number passed as an argument on the stack. The result is
;     stored in the AL register. It is designed to be called from
;     higher-level languages or other assembly routines.
;
; Parameters:
;     - port (on stack): The port number from which to read. Expected
;       to be passed in as the first argument on the stack.
;
; Returns:
;     - The byte read from the specified port is returned in the AL register.
;
; Usage:
;     To use this function, push the port number onto the stack and
;     call 'insb'. After the call, the AL register will contain the
;     byte read from the port.
insb:
    push ebp            ; Save the base pointer
    mov ebp, esp        ; Establish stack frame
    xor eax, eax        ; Clear EAX register (sets AL to 0)
    mov edx, [ebp + 8]  ; Move the first argument (port number) into EDX
    in al, dx           ; Read a byte from the port into AL
    pop ebp             ; Restore the base pointer
    ret                 ; Return to caller

; insw - Reads a word from a specified port into AX register.
;
; Description:
;     This function reads a word (two bytes) from the input port specified by the
;     port number passed as an argument on the stack. The result is stored in the AX register.
;     It is designed to be called from higher-level languages or other assembly routines.
;
; Parameters:
;     - port (on stack): The port number from which to read. Expected
;       to be passed in as the first argument on the stack.
;
; Returns:
;     - The word read from the specified port is returned in the AX register.
;
; Usage:
;     To use this function, push the port number onto the stack and
;     call 'insw'. After the call, the AX register will contain the
;     word read from the port.
insw:
    push ebp            ; Save the base pointer
    mov ebp, esp        ; Establish stack frame
    xor eax, eax        ; Clear EAX register (sets AX to 0)
    mov edx, [ebp + 8]  ; Move the first argument (port number) into EDX
    in ax, dx           ; Read a word from the port into AX
    pop ebp             ; Restore the base pointer
    ret                 ; Return to caller

; outb - Writes a byte to a specified port from AL register.
;
; Description:
;     This function writes a byte to the output port specified by the
;     port number passed as the first argument on the stack, with the data
;     to write passed as the second argument. The byte to write is taken from
;     the AL register. It is designed to be called from higher-level languages
;     or other assembly routines.
;
; Parameters:
;     - port (on stack): The port number to which to write. Expected
;       to be passed in as the first argument on the stack.
;     - data (on stack): The data byte to write. Expected to be passed
;       in as the second argument on the stack.
;
; Usage:
;     To use this function, push the data byte onto the stack, then
;     push the port number onto the stack, and call 'outb'. The function
;     will write the byte to the specified port.
outb:
    push ebp            ; Save the base pointer
    mov ebp, esp        ; Establish stack frame
    mov edx, [ebp + 8]  ; Move the first argument (port number) into EDX
    mov eax, [ebp + 12] ; Move the second argument (data) into EAX
    out dx, al          ; Write a byte to the port from AL
    pop ebp             ; Restore the base pointer
    ret                 ; Return to caller

; outw - Writes a word to a specified port from AX register.
;
; Description:
;     This function writes a word (two bytes) to the output port specified by the
;     port number passed as the first argument on the stack, with the data
;     to write passed as the second argument. The word to write is taken from
;     the AX register. It is designed to be called from higher-level languages
;     or other assembly routines.
;
; Parameters:
;     - port (on stack): The port number to which to write. Expected
;       to be passed in as the first argument on the stack.
;     - data (on stack): The data word to write. Expected to be passed
;       in as the second argument on the stack.
;
; Usage:
;     To use this function, push the data word onto the stack, then
;     push the port number onto the stack, and call 'outw'. The function
;     will write the word to the specified port.
outw:
    push ebp            ; Save the base pointer
    mov ebp, esp        ; Establish stack frame
    mov edx, [ebp + 8]  ; Move the first argument (port number) into EDX
    mov eax, [ebp + 12] ; Move the second argument (data) into EAX
    out dx, ax          ; Write a word to the port from AX
    pop ebp             ; Restore the base pointer
    ret                 ; Return to caller
