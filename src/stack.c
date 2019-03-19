//Linked list will be used to implement this stack
//
//check that these above include statements are necessary
#include"stack.h"

//return a new stack node
Node* stack_newNode(char *new_log)
{
    Node *new_node=(Node*)malloc(sizeof(Node));
    new_node->next=NULL;
    strcpy(new_node->log,new_log);
    return new_node;
}

//check if stack is empty
int stack_isEmpty(Node* node)
{
    return !node;
} 

//push a node into stack
void stack_push(Node** head_ref, char* new_log)
{
    Node *newNode=stack_newNode(new_log);
    newNode->next=*head_ref;
    *head_ref=newNode;
}

//pop a node from stack into a string
void stack_pop(Node** head_ref, char *ret_str)
{
    memset(ret_str,'\0',sizeof(ret_str));
    if(!stack_isEmpty(*head_ref))
    {
        strcpy(ret_str,(*head_ref)->log);
        Node *temp=*head_ref;
        *head_ref=(*head_ref)->next;
        free(temp);
    }
    
}

//peek the node into a string
void stack_peek(Node* top, char *ret_str) 
{ 
    if(!stack_isEmpty(top))
    {
        strcpy(ret_str,top->log);
    }
}

//as is its name describes
void stack_free(Node** head_ref)
{
    while(!stack_isEmpty(*head_ref)){
        Node *temp=*head_ref;
        *head_ref=(*head_ref)->next;
        free(temp);
    }
    
}
