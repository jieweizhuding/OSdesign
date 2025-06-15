// file.h
#ifndef _FILE_H
#define _FILE_H

#include "string.h"


struct filenode
{
    char *name;
    unsigned int size;
    unsigned int isdir;
    //type有0代表普通文件，1代表目录
    struct filenode *parent;
    struct filenode *child;
    struct filenode *prev;
    struct filenode *next;
};


struct filenode *initnode(char* n,int t);
void createroot();
// int run();
int findpath(char *topath);
void help();
int mkdir();
int create();
// int read();
// int write();
int del();
int rm();
int cd();
int dir();

#endif 