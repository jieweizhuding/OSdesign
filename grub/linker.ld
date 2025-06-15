ENTRY(_start)

/* 内核加载地址 1MB，并强制 .multiboot2 在前 32KB */
. = 0x100000;

SECTIONS {
    /* 显式将 .multiboot2 段放在最前面 */
    .multiboot2 : {
        *(.multiboot2)
    }

    /* 确保后续段不会覆盖 .multiboot2 */
    . = ALIGN(4K);  /* 按 4KB 对齐（可选） */

    /* 其他段（.text, .data, .bss 等） */
    .text : { *(.text) }
    .rodata : { *(.rodata) }
    .data : { *(.data) }
    .bss : { *(COMMON) *(.bss) }

    /DISCARD/ : { *(.eh_frame) *(.comment) }
}