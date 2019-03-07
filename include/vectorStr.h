#ifndef vectorSTR_H
#define vectorSTR_H

#define MAX_NODE_SIZE 50

typedef struct vectorStr_ {
    char** data;
    int size;
    int count;
} vectorStr;

void vectorStr_init(vectorStr*);
int vectorStr_count(vectorStr*);
void vectorStr_add(vectorStr *v, char *str);
void vectorStr_set(vectorStr*, int, char*);
char *vectorStr_get(vectorStr *v, int index, char *str);//returns string from vector at given index
void vectorStr_delete(vectorStr*, int);
void vectorStr_free(vectorStr*);
void vectorStr_cat(vectorStr *v1, vectorStr *v2);
//unsigned char vectorStr_contain(vectorStr *v, int p);
#endif