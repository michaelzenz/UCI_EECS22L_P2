
#include"queueChat.h"

//call this to init a queue
void queueChat_init(QueueChat *queue)
{
    queue->head=NULL;
    queue->tail=NULL;
    queue->size=0;
}
//as is its name describes
uchar queueChat_isEmpty(QueueChat *queue)
{
    return queue->size==0;
}
//as is its name describes
QNodeMsg* _newMsgNode(char *new_msg, char *srcUser)
{
    QNodeMsg *newNode;
    newNode=malloc(sizeof(QNodeMsg));
    newNode->msg=malloc(strlen(new_msg)+1);
    newNode->srcUser=malloc(strlen(srcUser)+1);
    newNode->next=NULL;

    strcpy(newNode->msg,new_msg);
    strcpy(newNode->srcUser,srcUser);

    return newNode;
}
//as is its name describes
QNodeChallenger* _newChallengerNode(char *challenger, char *host, int port)
{
    QNodeChallenger *newNode;
    newNode=malloc(sizeof(QNodeChallenger));
    newNode->challenger=malloc(strlen(challenger)+1);
    newNode->host=malloc(strlen(host)+1);
    newNode->next=NULL;

    strcpy(newNode->challenger,challenger);
    strcpy(newNode->host,host);
    newNode->port=port;

    return newNode;
}

//as is its name describes
//WARNING: please make sure you are calling the correct function
void queueChat_enqueueMsg(QueueChat *queue, char *new_msg, char *srcUser)
{
    QNodeMsg *newNode=_newMsgNode(new_msg,srcUser);
    if(queue->size==0)
    {
        queue->head=newNode;
        queue->tail=newNode;
    }
    else
    {
        ((QNodeMsg*)(queue->tail))->next=newNode;
        queue->tail=newNode;
    }
    queue->size++;
}

//as is its name describes
//WARNING: please make sure you are calling the correct function
void queueChat_enqueueChallenger(QueueChat *queue, char *challenger, char *host, int port)
{
    QNodeChallenger *newNode=_newChallengerNode(challenger,host,port);
    if(queue->size==0)
    {
        queue->head=newNode;
        queue->tail=newNode;
    }
    else
    {
        ((QNodeChallenger*)(queue->tail))->next=newNode;
        queue->tail=newNode;
    }
    queue->size++;
}
//as is its name describes
void queueChat_freeMsgNode(QNodeMsg *pNode)
{
    free(pNode->msg);
    free(pNode->srcUser);
}
//as is its name describes
void queueChat_freeChallengerNode(QNodeChallenger *pNode)
{
    free(pNode->challenger);
    free(pNode->host);
}

//pop a node from stack into a string
//WARNING: The node that this function returns must be free
//otherwise there will be memory leak
QNodeMsg queueChat_dequeueMsg(QueueChat *queue)
{
    if(queue->size==0){
        QNodeMsg dummy={NULL,NULL,NULL};
        return dummy;
    }
    QNodeMsg *temp=(QNodeMsg*)(queue->head);
    QNodeMsg retNode=*temp;

    
    if(queue->head==queue->tail)queue->tail=((QNodeMsg*)(queue->tail))->next;
    queue->head=((QNodeMsg*)(queue->head))->next;
    free(temp);
    queue->size--;
    return retNode;
}
//as is its name describes
QNodeChallenger queueChat_dequeueChallenger(QueueChat *queue)
{
    if(queue->size==0){
        QNodeChallenger dummy={NULL,NULL,-1,NULL};
        return dummy;
    }
    
    QNodeChallenger *temp=(QNodeChallenger*)(queue->head);
    QNodeChallenger retNode=*temp;
    
    if(queue->head==queue->tail)queue->tail=((QNodeChallenger*)(queue->tail))->next;
    queue->head=((QNodeChallenger*)(queue->head))->next;
    free(temp);
    queue->size--;
    return retNode;
}
//as is its name describes
void queueChat_printAllMsg(QueueChat *queue)
{
    if(queue->head==NULL)return;
    QNodeMsg *pNode=queue->head;
    printf("\nStart printing\n");
    while(pNode!=NULL){
        printf("from: %s, saying: %s\n",pNode->srcUser, pNode->msg);
        pNode=pNode->next;
    }
    printf("End printing\n\n");
}
//as is its name describes
void queueChat_printAllChallenger(QueueChat *queue)
{
    if(queue->head==NULL)return;
    QNodeChallenger *pNode=queue->head;
    printf("\nStart printing\n");
    while(pNode!=NULL){
        printf("challenger:%s, at:%s, using:%d\n",pNode->challenger, pNode->host,pNode->port);
        pNode=pNode->next;
    }
    printf("End printing\n\n");
}
//as is its name describes
void queueChat_freeMsgQueue(QueueChat *queue)
{
    if(queue->head==NULL)return;
    QNodeMsg *tmpNode;
    do{
        tmpNode=queue->head;
        queue->head=((QNodeMsg*)(queue->head))->next;
        queueChat_freeMsgNode(tmpNode);
    }while(queue->head!=NULL);
    queue->tail=NULL;
    queue->size=0;
}
//as is its name describes
void queueChat_freeChallengerQueue(QueueChat *queue)
{
    if(queue->head==NULL)return;
    QNodeChallenger *tmpNode;
    do{
        tmpNode=queue->head;
        queue->head=((QNodeChallenger*)(queue->head))->next;
        queueChat_freeChallengerNode(tmpNode);
    }while(queue->head!=NULL);
    queue->tail=NULL;
    queue->size=0;
}