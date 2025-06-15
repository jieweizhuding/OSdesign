#include "string.h"

int len(char *p){
    int i=0;
    while(p[i]!='\0'){
        i++;
    }
    return i;
}

//0是不同，1是相同

int strcmp(char *p,char* q){
    int size_p=len(p);
    int size_q=len(q);
    if(size_p!=size_q)
    return 0;
    int i=0;
    while(i<size_p){
        if(p[i]!=q[i]){
            return 0;
        }
        i++;
    }
    return 1;
}

void strcpy(char *p,char* q){
    //没有检查机制，p长度必须大于q
    int i=0;
    while(q[i]!='\0'){
        p[i]=q[i];
        i++;
    }
}

int strcat(char *p, char* q){
    int i=0;
    while(p[i]!='\0'){
        i++;
    }
    int j=0;
    while(q[j]!='\0'){
        p[i]=q[j];
        j++;
        i++;
    }
    p[i]='\0';
}