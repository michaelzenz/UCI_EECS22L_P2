#include"database.h"

map_void_t DataBaseMap;

//return a new node
User_Node* data_newNode(char *new_password, int port, bool new_online_status)
{
    User_Node *new_node=malloc(sizeof(User_Node));
    new_node->password=malloc(sizeof(new_password));
    strcpy(new_node->password, new_password);
    new_node->port = port;
    vectorStr_init(&new_node->friends);
    queueStr_init(&new_node->messages);
    queueStr_init(&new_node->challengers);
    new_node->online_status = new_online_status;
    return new_node;
}

//add a user to the database
void database_add_user(char* user_name, char* user_password, int port, bool user_online_status)
{
    User_Node *newNode = data_newNode(user_password, port, user_online_status);
    map_set(&DataBaseMap, user_name, (void*)newNode);
}

//reset the password of a user
void database_set_passwd(char* user, char* passwd)
{

}

//reset the port that a user uses to listen to other users
void database_set_port(char* user, int port)
{

}

//add a friend to a user`s friend list
void database_add_friend(char* user, char* friend)
{

}

//add a message to a user`s message queue
void database_add_msg(char* user, char* msg, char *srcUser)
{

}

//add a challenger to a user`s challenger queue
void database_add_challenger(char* user, char* challenger)
{

}

//set the online status of a user
void database_set_onlineStatus(char* user, bool onlineStatus)
{

}

//get the message queue of a user
QueueStr* database_get_msgQueue(char* user)
{

}

//get the next challenger
char* database_get_nextChallenger(char* user)
{

}