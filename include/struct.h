#ifndef STRUCT_H
#define STRUCT_H
#include"constant.h"
#include<assert.h>

typedef unsigned char uchar;

//the pack that contains username and password
typedef struct _PackUnamePasswd
{
    char UserName[30];
    char Password[30];
} PackUnamePasswd;

//the structure that serves as a vector of strings
typedef struct _vectorStr {
    char** data;
    int size;
    int count;
} vectorStr;

#endif