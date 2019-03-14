#ifndef vectorSTR_H
#define vectorSTR_H

typedef struct vectorStr_ {
    char** data;
    int size;
    int count;
    int totalLength;
} vectorStr;

void vectorStr_init(vectorStr*);
int vectorStr_count(vectorStr*);
int vectorStr_totalLength(vectorStr *v);
void vectorStr_add(vectorStr *v, char *str);
void vectorStr_set(vectorStr*, int, char*);
char* vectorStr_get(vectorStr*, int, char *str);
void vectorStr_delete(vectorStr*, int);
void vectorStr_free(vectorStr*);
void vectorStr_cat(vectorStr *v1, vectorStr *v2);
void vectorStr_printAll(vectorStr *v);
#endif