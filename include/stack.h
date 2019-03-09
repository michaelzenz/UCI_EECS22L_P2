#include<string.h>
#include"jsmn.h"
#include<stdio.h>
#include<stdlib.h>
#ifndef STACK_H
#define STACK_H

//the size of char array that should be used when 
//calling stack_ and convertion between move and string functions
#define STR_NODE_SIZE 80

typedef struct _Node
{
    char log[STR_NODE_SIZE];
    struct _Node *next;

}Node;

//return a new stack node
Node* stack_newNode(char *new_log);
//check if stack is empty
int stack_isEmpty(Node *node);
//push a node into stack
void stack_push(Node** head_ref, char* new_log);
//pop a node from stack into a string
void stack_pop(Node** head_ref, char* ret_str);
//peek the node into a string
void stack_peek(Node* top, char *ret_str) ;
//free the stack
void stack_free(Node** head_ref);

//Print Moves Log to MovesLog.txt
void stack_print_log(Node** head_ref);


#endif