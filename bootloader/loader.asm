org 0x9000      ; Loader 被加载到 0x9000 处

jmp start

; ---------- GDT 定义 ----------
gdt_start:
    ; 空描述符（必须）
    dd 0
    dd 0

    ; 代码段描述符（基址=0，限长=4GB，DPL=0）
    dw 0xFFFF       ; 段限长（低16位）
    dw 0x0000       ; 段基址（低16位）
    db 0x00         ; 段基址（中8位）
    db 0x9A         ; 访问权限（P=1, DPL=00, S=1, Type=1010）
    db 0xCF         ; 标志位（G=1, D/B=1, AVL=0, 段限长高4位=0xF）
    db 0x00         ; 段基址（高8位）

    ; 数据段描述符（基址=0，限长=4GB，DPL=0）
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x92         ; 访问权限（P=1, DPL=00, S=1, Type=0010）
    db 0xCF
    db 0x00
gdt_end:

; GDT 指针（供 lgdt 指令使用）
gdt_ptr:
    dw gdt_end - gdt_start - 1  ; GDT 长度
    dd gdt_start                ; GDT 基址

; 保护模式代码段选择子（RPL=00, TI=0, Index=1）
CODE_SELECTOR equ 0x08
DATA_SELECTOR equ 0x10

; ---------- 主程序 ----------
start:
    ; 初始化段寄存器
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x9000  ; 设置栈指针

    ; 打印提示信息（实模式）
    mov si, msg_loading
    call print

    ; 加载 GDT
    lgdt [gdt_ptr]

    ; 关闭中断
    cli

    ; 开启 A20 地址线（可选，某些 BIOS 默认开启）
    in al, 0x92
    or al, 0x02
    out 0x92, al

    ; 设置 CR0.PE=1，进入保护模式
    mov eax, cr0
    or eax, 0x01
    mov cr0, eax

    ; 远跳转刷新 CS 寄存器
    jmp dword CODE_SELECTOR:protected_mode

[bits 32]
protected_mode:
    ; 更新段寄存器
    mov ax, DATA_SELECTOR
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x9000  ; 32位栈指针

    ; 保护模式下打印信息（示例）
    mov esi, msg_protected
    call print32

    ; 加载内核（示例：从磁盘读取）
    ; call load_kernel

    ; 跳转到内核（假设内核入口在 0x100000）
    ; jmp 0x100000

    ; 无限循环（实际应跳转至内核）
    jmp $

; ---------- 实模式打印函数（16位） ----------
print:
    mov ah, 0x0E
.print_loop:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .print_loop
.done:
    ret

; ---------- 保护模式打印函数（32位） ----------
print32:
    mov edx, 0xB8000  ; 显存地址
.print_loop32:
    lodsb
    cmp al, 0
    je .done32
    mov [edx], al
    add edx, 2
    jmp .print_loop32
.done32:
    ret

; ---------- 数据段 ----------
msg_loading db "Loading...", 0
msg_protected db "Entered Protected Mode!", 0

; 填充至 512 字节（MBR 标准大小）
times 510 - ($ - $$) db 0
dw 0xAA55