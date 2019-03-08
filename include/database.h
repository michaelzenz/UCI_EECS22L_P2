#include<string.h>
#include"jsmn.h"
#include<stdlib.h>
#include"vectorStr.h"
#include"map.h"
#include"util.h"
#define USER_NODE_SIZE 80

#ifndef DATABASE_H
#define DATABASE_H

typedef struct _User_Node
{
    char* password;
	int socketnum;
    //queue challenger 
    //queue friendslist
    uchar online_status;

}User_Node;
User_Node* data_newNode(char *new_password, int new_socketnum, /*queue new_friendslist, queue new_challenger,*/ uchar new_online_status);
void database_add_user(map_int_t m, char* user_name, char* user_password, int user_socketnum, /*queue friendslist, queue new_challenger,*/ uchar user_online_status);

#endif