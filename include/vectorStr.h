
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef vectorSTR_H
#define vectorSTR_H

typedef struct vectorStr_ {
    char** data;
    int size;
    int count;
} vectorStr;

//same as vector.h
void vectorStr_init(vectorStr*);
int vectorStr_count(vectorStr*);
void vectorStr_add(vectorStr *v, char *str);
void vectorStr_set(vectorStr*, int, char*);
char* vectorStr_get(vectorStr *v, int index, char* str);
void vectorStr_delete(vectorStr*, int);
void vectorStr_free(vectorStr*);
void vectorStr_cat(vectorStr *v1, vectorStr *v2);
void vectorStr_printAll(vectorStr *v);
bool vectorStr_element_exists(vectorStr *v, char* str);
#endif