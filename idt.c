#include "idt.h"

static idt_entry_t idt[256];
static idt_ptr_t idt_ptr;

void idt_set_gate(int num, unsigned int base, unsigned short sel, unsigned char flags) {
    idt[num].base_low  = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel       = sel;
    idt[num].always0   = 0;
    idt[num].flags     = flags;
}

void idt_init(void) {
    idt_ptr.limit = (sizeof(idt_entry_t) * 256) - 1;
    idt_ptr.base  = (unsigned int)&idt;

    extern void isr0(void);
    extern void irq0_handler(void);
    extern void irq1_handler(void);
    extern void syscall_entry(void);

    idt_set_gate(0x00, (unsigned int)isr0,          0x08, 0x8E);
    idt_set_gate(0x20, (unsigned int)irq0_handler,  0x08, 0x8E);
    idt_set_gate(0x21, (unsigned int)irq1_handler,  0x08, 0x8E);
    idt_set_gate(0x80, (unsigned int)syscall_entry, 0x08, 0x8E);

    __asm__ volatile ("lidt %0" : : "m"(idt_ptr));
}