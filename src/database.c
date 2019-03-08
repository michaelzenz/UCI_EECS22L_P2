#include"database.h"




User_Node* data_newNode(char *new_password, int new_socketnum, /*queue new_friendslist, queue new_challenger,*/ uchar new_online_status)
{
    User_Node *new_node=malloc(sizeof(User_Node));
    new_node->password=malloc(sizeof(new_password));
    strcpy(new_node->password, new_password);
    new_node->socketnum = new_socketnum;
    new_node->online_status = new_online_status;
    //new_node->friendslist = new_friendslist;
    //new_node->challenger = new_challenger;
    return new_node;
}

void database_add_user(map_int_t m, char* user_name, char* user_password, int user_socketnum, /*queue friendslist, queue new_challenger,*/ uchar user_online_status)
{
    User_Node *newNode = data_newNode(user_password, user_socketnum, user_online_status);
    map_set(&m, user_name, newNode);
}

