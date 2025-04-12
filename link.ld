ENTRY(init)

MEMORY {
    FLASH   (rx) : ORIGIN = 0x0000, LENGTH = 8K
    SRAM    (rw) : ORIGIN = 0x0060, LENGTH = 512
}

SECTIONS {
    .text : {
        *(.vectors)
        *(.text)
        *(.rodata)
    } > FLASH

    .data : {
        *(.data)
    } > SRAM AT> FLASH

    .bss : {
        *(.bss)
    } > SRAM

    PROVIDE(uptime_ptr = .);
    . += 8;

    PROVIDE(stack_end = .);
    . += STACK_SIZE;
    PROVIDE(stack_start = .);
}
