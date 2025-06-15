[BITS 16]
[ORG 0x7C00]
start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov si, booting
    call print
    ; 读硬盘4个扇区到内存0x1000
    mov edi, 0x1000           ; 目标内存地址
    mov ecx, 2                ; 起始扇区号
    mov bl, 4                 ; 读取扇区数
    call read_disk
    ; 跳转到loader代码(0x1000段地址)
    jmp 0x0000:1000
; ---------------------------
; 打印字符串函数
print:
    mov ah, 0x0e
.next_char:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .next_char
.done:
    ret
booting:
    db "Booting System...", 10, 13, 0  ; \n\r0结尾字符串
; ---------------------------
; 读取扇区函数
 read_disk:
     ; 设置读写扇区的数量
     ; 0x1f2 是硬盘控制端口，表示读写扇区的数量
     mov dx, 0x1f2
     mov al, bl
     ; 写端口用OUT指令 将al的值写入到dx端口
     out dx, al
     inc dx; 0x1f3 起始扇区前8位端口
     ; 因为ecx为起始扇区 
     ; ecx中的cl就是0-7位
     mov al, cl; 起始扇区前8位
     out dx, al
     inc dx; 0x1f4 起始扇区中8位端口
     shr ecx, 8 ;右移8位
     mov al, cl; 起始扇区中8位
     out dx, al
     inc dx; 0x1f5 起始扇区高8位端口
     shr ecx, 8 ;右移8位
     mov al, cl; 起始扇区高8位
     out dx, al
     inc dx ;0x1f6
     shr ecx, 8
     and cl, 0b1111 ;将高4位置为0，对应起始扇区的24-27位
     mov al,0b1110_0000 ;第4位为0，表示主盘，第6位为1，表示LBA，5-7位必须为1
     ; 将al和cl合二为一，放在al中
     or al, cl
     out dx, al
     inc dx ;0x1f7
     mov al, 0x20 ;表示读硬盘
     out dx, al
     xor ecx, ecx ;清空ecx
     mov cl, bl ;得到写扇区的数量
     ; loop指令会检查ecx是否为0 cl在ecx里面
     .read:
         push cx ;保存下，因为函数里面使用了
         call .waits ;等待数据准备完毕
         call .reads ;读取一个扇区
         pop cx ;恢复
         loop .read
     ret
     .waits:
         mov dx, 0x1f7 ;读0x1f7端口
         .check:
             in al, dx ;将dx端口的值放入al中
             jmp $+2 ;直接跳转到下一行 其实什么都没做，就是为了延迟一下
             jmp $+2
             jmp $+2
             and al, 0b1000_1000 ;获得al的第3位和第7位
             cmp al, 0b0000_1000 ;测试是否第7位为0，第3位为1 硬盘不繁忙，数据准备完毕
             jnz .check ;数据没准备好
         ret
     .reads:
         mov dx, 0x1f0 ;用于读写数据
         mov cx, 256 ;一个扇区256字节
         ; loop指定会检查ecx cx在ecx里面
         .readw:
             in ax, dx
             jmp $+2 ;直接跳转到下一行 其实什么都没做，就是为了延迟一下
             jmp $+2
             jmp $+2
             ; edi表示读取的目标内存
             mov [edi], ax
             ; 因为ax是16bit，2个字节，所以edi+2
             add edi, 2
             loop .readw
         ret
; ---------------------------
; 引导扇区必须512字节并以0xAA55结尾
times 510 - ($ - $$) db 0
dw 0xAA55
