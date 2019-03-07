#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vectorStr.h"



void vectorStr_init(vectorStr *v)//intializes vector
{
    v->data = NULL;
    v->size = 0;
    v->count = 0;
}

int vectorStr_count(vectorStr *v)//returns count of vector
{
    return v->count;
}

void vectorStr_add(vectorStr *v, char *str)//add string to vector list
{
    if (v->size == 0) {
        v->size = 8; //not sure what this value should be
        v->data = malloc(sizeof(char*) * v->size);//first time allocating, allocate enough space for 8 char pointers
    }

    if (v->size == v->count) {//if we reach end of allocated space size == count
        v->size *= 2;
        v->data = realloc(v->data, sizeof(char*) * v->size);//everytime after first and we reach the total space already allocated, double the space allocated to vector
    }
    
    v->data[v->count] = malloc(sizeof(str)); //allocate space for string
    strcpy(v->data[v->count],str);//copy string into vector
    v->count++;
}

void vectorStr_set(vectorStr *v, int index, char *str)//changes value of string at given index of vector
{
    if (index >= v->count) {
        return;
    }

    strcpy(v->data[index], str);
}

char* vectorStr_get(vectorStr *v, int index, char** str)//edits str to become string from vector at given index
{
    if (index >= v->count) {
        return NULL;
    }
    if(*str==NULL) 
        *str=malloc(sizeof(v->data[index]));

    strcpy(*str, v->data[index]);
    return *(str);
}

//deletes element at given index
void vectorStr_delete(vectorStr *v, int index)
{
    if (index >= v->count) {
        return;
    }

    for (int i = index, j = index; i < v->count; i++,j++) {
        vectorStr_set(v,j,v->data[i]);
    }
    free(v->data[v->count-1]);
    v->count--;
}

void vectorStr_free(vectorStr *v)//frees memory of vector
{
    int size = v->count;
    for (int i=0; i< size; i++)
    {
        free(v->data[i]);
    }
    free(v->data);
}

void vectorStr_cat(vectorStr *v1, vectorStr *v2) //combines two vectors together
{
    char* tmp=NULL;
    for(int i=0;i<v2->count;i++)//adds every elements of v2 to end of v1
    {
        tmp = vectorStr_get(v2,i, &tmp);
        vectorStr_add(v1, tmp);
    }
}
