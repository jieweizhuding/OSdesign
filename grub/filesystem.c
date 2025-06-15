#include "filesystem.h"

#define NULL 0

void createroot()
{
    fs->recent = fs->root = initnode("/", 1);
    fs->root->parent = NULL;
    fs->root->child = NULL;
    fs->root->prev = fs->root->next = NULL;
    strcpy(fs->path, "/");
}

int dir()
{
    int i = 0, j = 0;
    struct filenode *temp;
    temp = fs->recent;
    if (temp == fs->root)
    {
        my_printf("      <DIR>                         %s\n", ".");
    }
    if (temp != fs->root)
    {
        my_printf("      <DIR>                         %s\n", "..");
        i++;
    }
    if (temp->child ==  0 )
    {
        my_printf("Total: %d directors      %d files\n", i, j);
        return 1;
    }
    temp = temp->child;
    while (temp)
    {
        if (temp->isdir==1)
        {
            my_printf("      <DIR>                        %s\n", temp->name);
            i++;
        }
        else if(temp->isdir==0)
        {
            my_printf("      <FILE>                       %s\n", temp->name);
            j++;
        }
        temp = temp->next;
    }
    my_printf("Total: %d directors      %d files\n", i, j);
    return 0;
}

int del()
{
    struct filenode *temp;
    char filename[25];
    my_scanf("%s", filename);
    temp = (struct filenode *)malloc(sizeof(struct filenode));

    if (fs->recent->child)
    {
        temp = fs->recent->child;
        while (temp->next && (strcmp(temp->name, filename) != 1 || temp->isdir != 0))
            temp = temp->next;
        if(temp== NULL){
            my_printf("不存在该文件！\n");
            return 0;
        }
        if (strcmp(temp->name, filename) != 1 || temp->isdir != 0)
        {
            my_printf("不存在该文件！\n");
            return 0;
        }
    }
    else
    {
        my_printf("不存在该文件！\n");
        return 0;
    }

    if (temp->parent == NULL)
    {
        temp->prev->next=temp->next;
        if(temp->next!=NULL)
        temp->next->prev=temp->prev;
    }
    else
    {
        if(temp->next!=NULL){
            temp->next->parent=fs->recent;
        }
        fs->recent->child = temp->next;
    }
    free(temp);
    my_printf("文件已删除!\n");
    return 0;
}

int rm()
{
    struct filenode *temp;
    char filename[25];
    my_scanf("%s", filename);
    temp = (struct filenode *)malloc(sizeof(struct filenode));

    if (fs->recent->child)
    {
        temp = fs->recent->child;
        while (temp->next && (strcmp(temp->name, filename) != 1 || temp->isdir != 1))
            temp = temp->next;
        if (strcmp(temp->name, filename) != 1 || temp->isdir != 1)
        {
            my_printf("不存在该目录！\n");
            return 0;
        }
    }
    else
    {
        my_printf("不存在该目录！\n");
        return 0;
    }

   if (temp->parent == NULL)
    {

        temp->prev->next=temp->next;
        if(temp->next!=NULL)
        temp->next->prev=temp->prev;
    }
    else
    {
        if(temp->next!=NULL){
            temp->next->parent=fs->recent;
        }
        fs->recent->child = temp->next;
    }
    free(temp);
    my_printf("目录已删除!\n");
    return 0;
}

int cd()
{
    char topath[30];
    my_scanf("%s", topath);
    if (strcmp(topath, ".") == 1)
        return 0;
    if (strcmp(topath, "..") == 1)
    {
        int i;
        while (fs->recent->prev)
            fs->recent = fs->recent->prev;
        if (fs->recent->parent)
        {
            fs->recent = fs->recent->parent;
        }

        i = len(fs->path);
        while (fs->path[i] != '/' && i > 0)
            i--;

        if (i != 0)
        {
            fs->path[i] = '\0';
        }
        else
        {
            fs->path[i + 1] = '\0';
        }
    }
    else
    {
        findpath(topath);
    }
    return 0;
}

int findpath(char *topath)
{
    struct filenode *temp;
    char temppath[30];
    unsigned int i = 0;
    int sign = 1;
    if (strcmp(topath, "/") == 1)
    {
        fs->recent = fs->root;
        strcpy(fs->path, "/");
        return 0;
    }
    temp = fs->recent;
    strcpy(temppath, fs->path);
    if (topath[0] == '/')
    {
        fs->recent = fs->root->child;
        i++;
        strcpy(fs->path, "/");
    }
    else
    {
        if (fs->recent != NULL && fs->recent != fs->root)
        {
            strcat(fs->path, "/");
        }

        if (fs->recent && fs->recent->child)
        {
            if (fs->recent->isdir)
                fs->recent = fs->recent->child;
            else
            {
                my_printf("路径错误！\n");
                return 1;
            }
        }
    }
    while (i <= len(topath) && fs->recent)
    {
        int j = 0;
        if (topath[i] == '/' && fs->recent->child)
        {
            i++;
            if (fs->recent->isdir)
                fs->recent = fs->recent->child;
            else
            {
                my_printf("路径错误\n");
                return 1;
            }
            strcat(fs->path, "/");
        }
        while (topath[i] != '/' && i <= len(topath))
        {
            fs->recentpath[j] = topath[i];
            i++;
            j++;
        }
        fs->recentpath[j] = '\0';
        while ((strcmp(fs->recent->name, fs->recentpath) != 1 || (fs->recent->isdir != 1)) && fs->recent->next != NULL)
        {
            fs->recent = fs->recent->next;
        }
        if (strcmp(fs->recent->name, fs->recentpath) == 1)
        {
            if (fs->recent->isdir == 0)
            {
                strcpy(fs->path, temppath);
                fs->recent = temp;
                my_printf("是文件不是目录。\n");
                return 1;
            }
            strcat(fs->path, fs->recent->name);
        }
        if (strcmp(fs->recent->name, fs->recentpath) != 1 || fs->recent == NULL)
        {
            strcpy(fs->path, temppath);
            fs->recent = temp;
            my_printf("输入路径错误\n");
            return 1;
        }
    }
    return 0;
}

int mkdir()
{
    struct filenode *temp;
    struct filenode* ttemp;
    temp = initnode(" ", 1);
    my_scanf("%s", temp->name);
    if (fs->recent->child == NULL)
    {
        temp->parent = fs->recent;
        temp->child = NULL;
        fs->recent->child = temp;
        temp->prev = temp->next = NULL;
        my_printf("目录建立成功!\n");
    }
    else
    {
        ttemp = fs->recent->child;
        if (strcmp(ttemp->name, temp->name) == 1 && ttemp->isdir == 1)
        {
            my_printf("目录已存在!\n");
            return 1;
        }
        while (ttemp->next)
        {
            ttemp = ttemp->next;
            if (strcmp(ttemp->name, temp->name) == 1 && ttemp->isdir == 1)
            {
                my_printf("目录已存在!\n");
                return 1;
            }
        }
        ttemp->next = temp;
        temp->parent = NULL;
        temp->child = NULL;
        temp->prev = ttemp;
        temp->next = NULL;
        my_printf("目录建立成功!\n");
    }
    return 0;
}

int create()
{
    struct filenode *temp;
    struct filenode* ttemp;
    temp = initnode(" ", 0);
    my_scanf("%s", temp->name);
    if (fs->recent->child == NULL)
    {
        temp->parent = fs->recent;
        temp->child = NULL;
        fs->recent->child = temp;
        temp->prev = temp->next = NULL;
        my_printf("文件创建成功!\n");
    }
    else
    {
        ttemp = fs->recent->child;
        if (strcmp(ttemp->name, temp->name) == 1 && ttemp->isdir == 0)
        {
            my_printf("文件已存在!\n");
            return 1;
        }
        while (ttemp->next)
        {
            ttemp = ttemp->next;
            if (strcmp(ttemp->name, temp->name) == 1 && ttemp->isdir == 0)
            {
                my_printf("文件已存在!\n");
                return 1;
            }
        }
        ttemp->next = temp;
        temp->parent = NULL;
        temp->child = NULL;
        temp->prev = ttemp;
        temp->next = NULL;
        my_printf("文件建立成功!\n");
    }
    return 0;
}