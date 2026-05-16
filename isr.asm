[BITS 32]
[GLOBAL isr0]
[GLOBAL irq0_handler]
[GLOBAL irq1_handler]
[GLOBAL syscall_entry]
[EXTERN isr_handler]
[EXTERN keyboard_handler]
[EXTERN process_yield]
[EXTERN syscall_handler]

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

irq0_handler:
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
    call process_yield
    pop gs
    pop fs
    pop es
    pop ds
    popa
    mov al, 0x20
    out 0x20, al
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

syscall_entry:
    push ebx
    push ecx
    push edx
    push eax
    call syscall_handler
    add esp, 16
    iret