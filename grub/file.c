#include "file.h"

#define NULL 0


// 创建文件或目录的存储节点
struct filenode *initnode(char* n,int t)
{
    struct filenode *node = (struct filenode *)malloc(sizeof(struct filenode));
    node->name = (char*)kmalloc(len(n)+1);
	strcpy(node->name,n);
	node->name[len(n)]=0;

    node->isdir=t;
    node->size=0;
    node->parent = NULL;
    node->child = NULL;
    node->prev = NULL;
    node->next = NULL;
    return node;
}


// void help()
// {
//     printf("\n");
//     printf("create:             建立文件。\n");
//     printf("read:               读取文件。\n");
//     printf("write:              写入文件。\n");
//     printf("delete:             删除文件。\n");
//     printf("rm:                 删除目录。\n");
//     printf("mkdir:              建立目录。\n");
//     printf("cd:                 切换目录。\n");
//     printf("dir:                显示目录。\n");
//     printf("logout:             退出登录。\n");
// }


/*
int read()
{
    char filename[FILENAME_LENGTH];
    scanf("%s", filename);
    if (fs->recent->child == NULL)
    {
        printf("文件不存在!\n");
        return 1;
    }
    if (strcmp(fs->recent->child->filename, filename) == 0)
    {
        printf("%s\n",fs->recent->child->content);
        return 0;
    }
    else
    {
        temp = fs->recent->child;
        while (temp->next)
        {
            if (strcmp(temp->next->filename, filename) == 0)
            {
                printf("%s\n",temp->next->content);
                return 0;
            }
            temp=temp->next;
        }
        printf("文件不存在!\n");
        return 1;
    }
}

*/
/*
int write()
{
    char filename[FILENAME_LENGTH];
    scanf("%s", filename);
    if (fs->recent->child == NULL)
    {
        printf("文件不存在!\n");
        return 1;
    }
    if (strcmp(fs->recent->child->filename, filename) == 0)
    {
        scanf("%s",fs->recent->child->content);
        printf("文件写入成功!\n");
        return 0;
    }
    else
    {
        temp = fs->recent->child;
        while (temp->next)
        {
            if (strcmp(temp->next->filename, filename) == 0)
            {
                scanf("%s",temp->next->content);
                printf("文件写入成功!\n");
                return 0;
            }
            temp=temp->next;
        }
        printf("文件不存在!\n");
        return 1;
    }
}
*/











/*
int run()
{
    printf("filesystem:%s>", path);
    scanf("%s", command);
    if (strcmp(command, "mkdir") == 0)
    mkdir();
    else if (strcmp(command, "dir") == 0)
    dir();
    else if (strcmp(command, "cd") == 0)
    cd();
    else if (strcmp(command, "create") == 0)
    create();
    else if (strcmp(command, "read") == 0)
    read();
    else if (strcmp(command, "rm") == 0)
    rm();
    else if (strcmp(command, "write") == 0)
    write();
    else if (strcmp(command, "delete") == 0)
    del();
    else if (strcmp(command, "help") == 0)
    help();
    else if (strcmp(command, "logout") == 0)
    return 0;
    else
    printf("请参考help提供的命令列表!\n");
    return 1;
}


*/