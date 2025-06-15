// kernel.c

#include "show.h"
#include "string.h"
#include "filesystem.h"

// Multiboot2 头部（必须放在内核文件的前 32KB 内）
const uint32_t multiboot_header[] = {
    0xE85250D6,                 // Magic
    0x00000000,                 // Architecture (0 = 32-bit)
    0x0000001C,                 // Header length
    0x100000000 - (0xE85250D6 + 0x0 + 0x1C), // Checksum
    0x00000000,                 // END tag
    0x00000008,
};


void _start(void) {
    startup();
    createroot();
    char command[120];
    while (1){// 防止返回
        promptString();
        my_scanf("%s",command);
        if(strcmp(command,"clear")){
            clear_screen();
        }else if(strcmp(command,"poweroff")){
            power_off_apm();
        }else if(strcmp(command,"cd")){
            cd();
        }else if(strcmp(command,"create")){
            create();
        }else if(strcmp(command,"delete")){
            del();
        }else if(strcmp(command,"dir")){
            dir();
        }else if(strcmp(command,"mkdir")){
            mkdir();
        }else if(strcmp(command,"rm")){
            rm();
        }
    }
}