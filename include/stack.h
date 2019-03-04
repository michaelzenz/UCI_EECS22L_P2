#include<string.h>
#include"jsmn.h"
#include"struct.h"
#include<stdio.h>
#include<stdlib.h>
#ifndef STACK_H
#define STACK_H

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
//convert move to string
void move2string(char* str_move, Move *move);
//convert and return a move from a string
Move string2move(char* str_move);

//Print Moves Log to MovesLog.txt
void stack_print_log(Node** head_ref);


#endif