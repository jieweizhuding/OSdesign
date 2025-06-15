#include"libc.h"

#define ACPI_PM1a_CNT 0x804
#define ACPI_SHUTDOWN_CMD 0x2000
// VGA 缓冲区地址
volatile uint16_t *vga_buffer = (volatile uint16_t*)0xB8000;

// 当前光标位置（行列）
int cursor_row = 0;
int cursor_col = 0;

// 简单的扫描码到ASCII映射表（只处理字母和数字）
static const char scan_code_map[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=','\b', // 0x00-0x0E
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',    // 0x0F-0x1C
    0,  'a','s','d','f','g','h','j','k','l',';','\'','`',         // 0x1D-0x29
    0,  '\\','z','x','c','v','b','n','m',',','.','/',0,           // 0x2A-0x36
    '*', 0,  ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,          // 0x37-0x45
    // 剩余部分保持为0
};

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t data) {
    __asm__ volatile ("outb %0, %1" : : "a"(data), "Nd"(port));
}


void power_off_apm() {
    // 进入 APM 模式
    outb(0x70, 0x8F);  // 关闭 NMI
    outb(0x71, 0x00);
    // 发送 APM 关机命令
    asm volatile (
        "mov $0x5301, %%ax\n"  // APM 连接
        "xor %%bx, %%bx\n"
        "int $0x15\n"
        "mov $0x530E, %%ax\n"  // APM 驱动版本
        "xor %%bx, %%bx\n"
        "int $0x15\n"
        "mov $0x5307, %%ax\n"  // 关机
        "mov $0x01, %%bx\n"
        "mov $0x03, %%cx\n"
        "int $0x15"
        ::: "eax", "ebx", "ecx"
    );
    while (1);
}


void move_cursor(int row, int col) {
    uint16_t pos = row * 80 + col;
    outb(0x3D4, 0x0F);          // 设置光标低字节
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);          // 设置光标高字节
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

// 输出一个字符（带颜色）
void putchar(char c, uint8_t color) {
    if (c == '\n') {
        // 换行：移动到下一行首
        cursor_col = 0;
        cursor_row++;
    } else {
        // 写入字符到显存
        vga_buffer[cursor_row * 80 + cursor_col] = (color << 8) | c;
        cursor_col++;
        if (cursor_col >= 80) {
            cursor_col = 0;
            cursor_row++;
        }
    }

    // 处理屏幕滚动（超出底部时上移）
    if (cursor_row >= 25) {
        // 将所有行向上移动一行
        for (int i = 0; i < 24 * 80; i++) {
            vga_buffer[i] = vga_buffer[i + 80];
        }
        // 清空最后一行
        for (int i = 24 * 80; i < 25 * 80; i++) {
            vga_buffer[i] = (color << 8) | ' ';
        }
        cursor_row = 24;
    }
    move_cursor(cursor_row,cursor_col);
}

// 等待并读取一个键盘输入
char getchar() {
    uint8_t status;
    uint8_t scan_code;
    char c;

    while (1) {
        // 等待键盘输出缓冲区有数据
        do {
            status = inb(0x64);
        } while ((status & 0x01) == 0);

        scan_code = inb(0x60);

        // 忽略释放键（高位为 1）
        if (scan_code & 0x80) {
            continue;
        }

        c = scan_code_map[scan_code];
        if (c != 0) {
            return c;
        }
        // 否则继续等待下一个合法按键
    }
}

void clear_screen() {
    volatile uint16_t *vga = vga_buffer;
    for (int i = 0; i < 80 * 25; i++) {
        vga[i] = ' ' | (0x00f << 8);  // 空格 + 属性
    }
    cursor_row=0;
    cursor_col=0;
    move_cursor(cursor_row,cursor_col);
}




void my_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 's': {  // 字符串
                    char *s = va_arg(args, char*);
                    while (*s) putchar(*s++, 0x0F);
                    break;
                }
                case 'c': {  // 字符
                    char c = va_arg(args, int);
                    putchar(c, 0x0F);
                    break;
                }
                case 'd': {  // 整数
                    int num = va_arg(args, int);
                    char buffer[32];
                    int i = 0;
                    
                    // 处理负数
                    if (num < 0) {
                        putchar('-', 0x0F);
                        num = -num;
                    }
                    
                    // 转换为字符串（反向）
                    do {
                        buffer[i++] = '0' + (num % 10);
                        num /= 10;
                    } while (num > 0);
                    
                    // 反向输出
                    while (i > 0) {
                        putchar(buffer[--i], 0x0F);
                    }
                    break;
                }
            }
        } else {
            putchar(*fmt, 0x0F);
        }
        fmt++;
    }

    va_end(args);
    move_cursor(cursor_row,cursor_col);
}

void handle_backspace(uint8_t color) {
    if (cursor_col > 0) {
        cursor_col--;  // 左移一列
    } else if (cursor_row > 0) {
        cursor_row--;  // 跳到上一行末尾
        cursor_col = 79;
    }
    // 用空格覆盖原字符
    vga_buffer[cursor_row * 80 + cursor_col] = (color << 8) | ' ';
    move_cursor(cursor_row, cursor_col);  // 更新光标位置
}

// 简易版 scanf（仅支持 %s）
int my_scanf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int matched = 0;

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 's': {
                    char *buf = va_arg(args, char*);
                    int i = 0;
                    while (1) {
                        char c = getchar();  // 假设已实现 getchar()
                        if (c == '\n') {     // 回车结束输入
                            buf[i] = '\0';
                            matched++;
                            putchar('\n',0x0F);  // 换行
                            break;
                        } else if (c == '\b') {  // 退格键
                            if (i > 0) {
                                i--;
                                handle_backspace(0x0F);  // 白底黑字
                            }
                        } else {
                            buf[i++] = c;
                            putchar(c,0x0F);  // 回显字符
                        }
                    }
                    break;
                }
            }
        }
        fmt++;
    }
    va_end(args);
    return matched;
}