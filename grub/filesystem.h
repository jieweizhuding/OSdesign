// filesystem.h
#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H

#include "file.h"
#include "./kernel/libc.h"
struct filesystem
{
    struct filenode *root, *recent;
    char path[50], recentpath[50];
}*fs;

void createroot();
int dir();
int del();
int rm();
int cd();
int mkdir();
int create();

#endif 