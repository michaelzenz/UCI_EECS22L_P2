#include"database.h"
#include"codec.h"


typedef struct _User_Node
{
    char* password;
    char* host;
	int port;
    vectorStr friends;
    QueueChat messages;
    QueueChat challengers;
    bool online_status;
}User_Node;

map_void_t DataBaseMap;

//intializes database
void database_intialize()
{
    map_init(&DataBaseMap);
}

//return a new node
User_Node* data_newNode(char *new_password, int port, bool new_online_status)
{
    User_Node *new_node=malloc(sizeof(User_Node));
    new_node->password=malloc(strlen(new_password)+1);
    strcpy(new_node->password, new_password);
    new_node->host=NULL;
    new_node->port = port;
    vectorStr_init(&new_node->friends);
    queueChat_init(&new_node->messages);
    queueChat_init(&new_node->challengers);
    new_node->online_status = new_online_status;
    return new_node;
}

//add a user to the database
void database_add_user(char* user_name, char* user_password, int port, bool user_online_status)
{
    User_Node *newNode = data_newNode(user_password, port, user_online_status);
    map_set(&DataBaseMap, user_name, newNode);
}

//reset the password of a user
void database_set_passwd(char* user, char* passwd)
{
    User_Node *node = map_get(&DataBaseMap, user);
    node->password=realloc(node->password, strlen(passwd));
    strcpy(node->password, passwd);
}

//reset the password of a user
void database_set_host(char* user, char* host)
{
    User_Node *node = map_get(&DataBaseMap, user);
    if(node->host==NULL)node->host=malloc(1+strlen(host));
    else node->host=realloc(node->host, 1+strlen(host));
    strcpy(node->host, host);
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
    queueChat_enqueueMsg(&(node->messages), msg, srcUser);
}

//add a challenger to a user`s challenger queue
void database_add_challenger(char* user, char* challenger)
{
    User_Node *node = map_get(&DataBaseMap, user);
    queueChat_enqueueChallenger(&(node->challengers), 
        challenger, database_get_host(challenger),
        database_get_port(challenger));
}

//set the online status of a user
void database_set_onlineStatus(char* user, bool onlineStatus)
{
    User_Node *node = map_get(&DataBaseMap, user);
    node->online_status = onlineStatus;
}

//return false if no such friend
bool database_delete_friends(char* user, char *friend)
{
    User_Node *node = map_get(&DataBaseMap, user);
    vectorStr friends=node->friends;
    int friendNb=vectorStr_count(&friends);
    char temp[MAX_USERNAME_LEN];

    int i=0;
    for(;i<friendNb;i++)
    {
        if(!strcmp(vectorStr_get(&friends,i,temp),friend)){
            vectorStr_delete(&friends,i);
            break;
        }
    }
    if(i==friendNb)return false;
}

//get the message queue of a user
QueueChat* database_get_msgQueue(char* user)
{
    User_Node *node = map_get(&DataBaseMap, user);
    return &(node->messages);
}

//get the next challenger
QNodeChallenger database_get_nextChallenger(char* user)
{
    User_Node *node = map_get(&DataBaseMap, user);
    QNodeChallenger qnode=queueChat_dequeueChallenger(&(node->challengers));
    return qnode;
}

//test functions
char* database_get_password(char* user)
{
    User_Node *node = map_get(&DataBaseMap, user);
    return node->password;
}

char* database_get_host(char* user)
{
    User_Node *node = map_get(&DataBaseMap, user);
    return node->host;
}

int database_get_port(char* user)
{
    User_Node *node = (User_Node*)map_get(&DataBaseMap, user);
    return node->port;
}

bool database_isUserExist(char* user)
{
    User_Node *node=map_get(&DataBaseMap,user);
    return node!=NULL;
}

vectorStr database_get_friends(char* user)
{
    User_Node *node = (User_Node*)map_get(&DataBaseMap, user);
    return node->friends;
}

bool database_get_onlineStatus(char* user)
{
    User_Node *node = (User_Node*)map_get(&DataBaseMap, user);
    return node->online_status;
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

char* encodeDatabaseNode()
{
    
}