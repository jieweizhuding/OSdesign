// libc.h
#ifndef _LIBC_H
#define _LIBC_H

#include <stdint.h>
#include <stdarg.h>

// 函数声明
void putchar(char c, uint8_t color);
void my_printf(const char *fmt, ...);
char getchar();
int my_scanf(const char *fmt, ...);
void clear_screen();
void power_off_apm();

#endif // _LIBC_H