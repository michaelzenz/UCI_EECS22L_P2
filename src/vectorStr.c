

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
    
    v->data[v->count] = malloc(strlen(str)+1); //allocate space for string
    strcpy(v->data[v->count],str);//copy string into vector
    v->count++;
}


void vectorStr_set(vectorStr *v, int index, char *str)//changes value of string at given index of vector
{
    if (index >= v->count) {
        return;
    }
    if(1+strlen(v->data[index])<strlen(str)+1)
        v->data[index]=realloc(v->data[index],strlen(str)+1);
    strcpy(v->data[index], str);
}

char* vectorStr_get(vectorStr *v, int index, char* str)//edits str to become string from vector at given index
{
    if (index >= v->count) {
        return NULL;
    }

    strcpy(str, v->data[index]);
    return str;
}

char* _vectorStr_get(vectorStr *v, int index, char** str)
{
    if (index >= v->count) {
        return NULL;
    }
    if(*str==NULL)
        *str=malloc(strlen(v->data[index])+1);
    else if(strlen(*str)+1<strlen(v->data[index])+1)
        *str=realloc(*str,strlen(v->data[index])+1);
    vectorStr_get(v,index,*str);
    return *str;
}

//deletes element at given index
void vectorStr_delete(vectorStr *v, int index)
{
    if (index >= v->count) {
        return;
    }

    int cnt=v->count;
    for (int i = index, j = index+1; i < cnt-1; i++,j++) {
        vectorStr_set(v,i,v->data[j]);
    }
    free(v->data[cnt-1]);
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
        vectorStr_add(v1, _vectorStr_get(v2,i, &tmp));
    }
    free(tmp);
}

void vectorStr_printAll(vectorStr *v)
{
    int cnt=v->count;
    printf("start printing\n");
    for(int i=0;i<cnt;i++)printf("%s  ",v->data[i]);
    printf("\nend printing\n");
}

bool vectorStr_element_exists(vectorStr *v, char* str)
{
    int cnt=v->count;
    for(int i=0; i<cnt; i++)
    {
        if (!strcmp(str, v->data[i]))
            return true;
    }
    return false;
}