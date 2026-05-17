ENTRY(_start)

SECTIONS {
    . = 0x8000;

    .text : {
        entry.o(.text)
        *(.text)
    }

    .data : {
        *(.data)
        *(.rodata*)
    }

    .bss : {
        *(.bss)
    }
}