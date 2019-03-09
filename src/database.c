#include"database.h"



typedef struct _User_Node
{
    char* password;
	int port;
    vectorStr friends;
    QueueStr messages;
    QueueStr challengers;
    bool online_status;

}User_Node;

typedef map_t(User_Node*) DataBaseMap_t;

DataBaseMap_t DataBaseMap;

//intializes database
void database_intialize()
{
    map_init(&DataBaseMap);
}


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
    void *test=(void*)newNode;
    map_set(&DataBaseMap, user_name, newNode);
}

//reset the password of a user
void database_set_passwd(char* user, char* passwd)
{
    User_Node *node = map_get(&DataBaseMap, user);
    realloc(node->password, sizeof(passwd));
    strcpy(node->password, passwd);
}

//reset the port that a user uses to listen to other users
void database_set_port(char* user, int port)
{
    User_Node *node = map_get(&DataBaseMap, user);
    node->port = port;
}

//add a friend to a user`s friend list
void database_add_friend(char* user, char* friend)
{
    User_Node *node = map_get(&DataBaseMap, user);
    vectorStr_add(&(node->friends), friend);
}

//add a message to a user`s message queue
void database_add_msg(char* user, char* msg, char *srcUser)
{
    User_Node *node = map_get(&DataBaseMap, user);
    queueStr_enqueue(&(node->messages), msg, srcUser);
}

//add a challenger to a user`s challenger queue
void database_add_challenger(char* user, char* challenger)
{
    User_Node *node = map_get(&DataBaseMap, user);
    queueStr_enqueue(&(node->challengers), challenger, challenger);
}

//set the online status of a user
void database_set_onlineStatus(char* user, bool onlineStatus)
{
    User_Node *node = map_get(&DataBaseMap, user);
    node->online_status = onlineStatus;
}

//get the message queue of a user
QueueStr* database_get_msgQueue(char* user)
{
    User_Node *node = map_get(&DataBaseMap, user);
    return &(node->messages);
}

//get the next challenger
char* database_get_nextChallenger(char* user)
{
    User_Node *node = map_get(&DataBaseMap, user);
    char* challenger;
    queueStr_dequeue(&(node->challengers), challenger);
    return challenger;
}

//test functions
char* database_get_password(char* user)
{
    User_Node *node = map_get(&DataBaseMap, user);
    return node->password;
}

int database_get_port(char* user)
{
    User_Node *node = (User_Node*)map_get(&DataBaseMap, user);
    return node->port;
}

void test_database()
{
    database_intialize();
    printf("...testing...\n");
    database_add_user("keenan", "password", 11009, 1);
    const char *key;
    map_iter_t iter=map_iter(&DataBaseMap);

    while((key=map_next(&DataBaseMap,&iter))){
        //void *test=map_get(&DataBaseMap,key);
        User_Node *node=map_get(&DataBaseMap,key);
        int hit=1;
    }
    
}