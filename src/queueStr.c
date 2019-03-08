
#include"queueStr.h"

QNodeStr queueStr_init(QueueStr *queue)
{
    queue->head=NULL;
    queue->tail=NULL;
    queue->size=0;
}

uchar stack_isEmpty(QueueStr *queue)
{
    return queue->size<=0;
}

void queueStr_enqueue(QueueStr *queue, char *new_msg, char *srcUser)
{
    if(queue->size==0)
    {
        QNodeStr *head=queue->head;
        head=malloc(sizeof(QNodeStr));
        head->msg=malloc(sizeof(new_msg));
        head->srcUser=malloc(sizeof(srcUser));
        head->next=NULL;
        strcpy(head->msg,new_msg);
        strcpy(head->srcUser,srcUser);
        queue->tail=head;
        queue->head=head;
    }
    else
    {
        QNodeStr *tail=queue->tail;
        tail->next=malloc(sizeof(QNodeStr));
        tail=tail->next;
        tail->msg=malloc(sizeof(new_msg));
        tail->srcUser=malloc(sizeof(srcUser));
        tail->next=NULL;
        strcpy(tail->msg,new_msg);
        strcpy(tail->srcUser,srcUser);
        queue->tail=tail;
    }
    queue->size++;
}

void queueStr_dequeue(QueueStr *queue, char *ret_str)
{
    if(queue->head==NULL)return;
    QNodeStr *head=queue->head;
    strcpy(ret_str,head->msg);
    if(queue->head==queue->tail)queue->tail=queue->tail->next;
    queue->head=queue->head->next;
    free(head->msg);
    free(head->srcUser);
    free(head);
}

void queueStr_printAll(QueueStr *queue)
{
    if(queue->head==NULL)return;
    QNodeStr *pNode=queue->head;
    printf("\nStart printing\n");
    while(pNode!=NULL){
        printf("from: %s, saying: %s\n",pNode->srcUser, pNode->msg);
        pNode=pNode->next;
    }
    printf("End printing\n\n");
}

void queueStr_free(QueueStr *queue)
{
    if(queue->head==NULL)return;
    QNodeStr *tmpNode;
    do{
        tmpNode=queue->head;
        queue->head=queue->head->next;
        free(tmpNode->msg);
        free(tmpNode->srcUser);
        free(tmpNode);
    }while(queue->head!=NULL);
    queue->tail=NULL;
    queue->size=0;
}