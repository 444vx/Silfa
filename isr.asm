[BITS 32]
[GLOBAL isr0]
[GLOBAL irq1_handler]
[EXTERN isr_handler]
[EXTERN keyboard_handler]

isr0:
    cli
    push byte 0
    push byte 0
    jmp isr_common

isr_common:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call isr_handler
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    sti
    iret

irq1_handler:
    cli
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call keyboard_handler
    pop gs
    pop fs
    pop es
    pop ds
    popa
    sti
    iret