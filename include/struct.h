#ifndef STRUCT_H
#define STRUCT_H
#include"constant.h"
#include<assert.h>

typedef unsigned char uchar;

typedef struct _PackUnamePasswd
{
    char UserName[30];
    char Password[30];
} PackUnamePasswd;

typedef struct _vectorStr {
    char** data;
    int size;
    int count;
} vectorStr;

#endif