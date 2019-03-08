#include<string.h>
#include"jsmn.h"
#include<stdlib.h>
#include"vectorStr.h"
#include"map.h"
#include"util.h"
#include"queueStr.h"

#ifndef DATABASE_H
#define DATABASE_H

typedef struct _User_Node
{
    char* password;
	int port;
    vectorStr friends;
    QueueStr messages;
    QueueStr challengers;
    bool online_status;

}User_Node;

//add a user to the database
void database_add_user( char* user_name, char* user_password, int port, bool user_online_status);

//reset the password of a user
void database_set_passwd(char* user, char* passwd);

//reset the port that a user uses to listen to other users
void database_set_port(char* user, int port);

//add a friend to a user`s friend list
void database_add_friend(char* user, char* friend);

//add a message to a user`s message queue
void database_add_msg(char* user, char* msg, char *srcUser);

//add a challenger to a user`s challenger queue
void database_add_challenger(char* user, char* challenger);

//set the online status of a user
void database_set_onlineStatus(char* user, bool onlineStatus);

//get the message queue of a user
QueueStr* database_get_msgQueue(char* user);

//get the next challenger
char* database_get_nextChallenger(char* user);
#endif