#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"util.h"
#ifndef QUEUE_STR_H
#define QUEUE_STR_H


typedef struct _QNodeStr
{
    char *msg;
    char *srcUser;
    struct _QNodeStr *next;
}QNodeStr;

typedef struct _QueueStr{
    QNodeStr *head,*tail;
    int size;
}QueueStr;

//init queue
QNodeStr queueStr_init(QueueStr *queue);
//check if stack is empty
uchar queueStr_isEmpty(QueueStr *queue);
//push a node into stack
void queueStr_enqueue(QueueStr *queue, char *new_msg, char *srcUser);
//pop a node from stack into a string
//WARNING: The node that this function returns must be free
//otherwise there will be memory leak
QNodeStr queueStr_dequeue(QueueStr *queue);
//frees a node
void queueStr_freeNode(QNodeStr *pNode);
//Print Moves Log to MovesLog.txt
void queueStr_printAll(QueueStr *queue);
//free queue
void queueStr_free(QueueStr *queue);

#endif