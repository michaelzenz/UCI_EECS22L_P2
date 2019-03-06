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

    v->data[index] = str;
}

char *vectorStr_get(vectorStr *v, int index)//returns string from vector at given index
{
    if (index >= v->count) {
        return NULL;
    }

    return v->data[index];
}

void vectorStr_delete(vectorStr *v, int index)//deletes element at given index
{
    if (index >= v->count) {
        return;
    }

    for (int i = index, j = index; i < v->count; i++) {
        v->data[j] = v->data[i];
        j++;
    }

    v->count--;
}

void vectorStr_free(vectorStr *v)//frees memory of vector
{
    free(v->data);
}

void vectorStr_cat(vectorStr *v1, vectorStr *v2) //combines two vectors together
{
    for(int i=0;i<v2->count;i++)//adds every elements of v2 to end of v1
    {
        vectorStr_add(v1,vectorStr_get(v2,i));
    }
}

/*unsigned char vectorStr_contain(vectorStr *v, int str)//checks if element is contained in vector
{
    int cnt=v->count;
    for(int i=0;i<cnt;i++)
    {
        if(vectorStr_get(v,i)==str)return 1;
    }
    return 0;
}
*/