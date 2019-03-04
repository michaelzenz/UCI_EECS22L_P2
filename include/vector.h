#ifndef VECTOR_H
#define VECTOR_H

typedef struct vector_ {
    int* data;
    int size;
    int count;
} vector;

//init a vector
void vector_init(vector*);
//count how many elements in vector
int vector_count(vector*);
//add an element to vector
void vector_add(vector*, int);
//set element at index to be new_val
void vector_set(vector*, int index, int new_val);
//get element at index
int vector_get(vector*, int index);
//delete element at index
void vector_delete(vector*, int index);
//free the vector
void vector_free(vector*);
//concatenate the elements in v2 to v1
void vector_cat(vector *v1, vector *v2);
//check if v contains element p
unsigned char vector_contain(vector *v, int p);
#endif