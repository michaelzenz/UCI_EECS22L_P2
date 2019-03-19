
/*********************/
//WARNING:
//this queue is only bulit for kchat-server
/*********************/

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"util.h"

#ifndef QUEUE_STR_H
#define QUEUE_STR_H

//A message node
typedef struct _QNodeMsg
{   
    char *msg;//message
    char *srcUser;//from who
    struct _QNodeMsg *next;//next node
}QNodeMsg;

//a challenger node
typedef struct _QNodeChallenger
{
    char *challenger;
    char *host;
    int port;
    struct _QNodeChallenger *next;
}QNodeChallenger;

//a struct of queue
typedef struct _QueueChat{
    void *head,*tail;//head: first node, tail: last node
    int size;
}QueueChat;

//init queue
void queueChat_init(QueueChat *queue);
//check if stack is empty
uchar queueChat_isEmpty(QueueChat *queue);
//push a node into stack
void queueChat_enqueueMsg(QueueChat *queue, char *new_msg, char *srcUser);
//enqueue a challenger
void queueChat_enqueueChallenger(QueueChat *queue, char *challenger, char *host, int port);

//pop a node from stack into a string
//WARNING: The node that this function returns must be free
//otherwise there will be memory leak
QNodeMsg queueChat_dequeueMsg(QueueChat *queue);
QNodeChallenger queueChat_dequeueChallenger(QueueChat *queue);

//Print The queue for diffetent queues
void queueChat_printAllMsg(QueueChat *queue);
void queueChat_printAllChallenger(QueueChat *queue);

//free the node
void queueChat_freeMsgNode(QNodeMsg *pNode);
void queueChat_freeChallengerNode(QNodeChallenger *pNode);

//free queue
void queueChat_freeMsgQueue(QueueChat *queue);
void queueChat_freeChallengerQueue(QueueChat *queue);

#endif