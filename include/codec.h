
#include"struct.h"
#include<string.h>
#include"jsmn.h"
#include<stdlib.h>
#include"vectorStr.h"

#ifndef CODEC_H
#define CODEC_H

#define MAX_JSON_OBJ_LEN 50

#define MAX_USERNAME_LEN 30
#define MAX_PASSWD_LEN 30

//for PackUnamePasswd

#define MAX_PUP_SIZE 250

#define LOGIN 0
#define REGISTER 1

//for PackAnswerLR
#define MAX_PALR_SIZE (10+10+MAX_FRIEND_NB*MAX_USERNAME_LEN+10)

#define MAX_FRIEND_NB 2

//for PackQuery
#define MAX_PQ_SIZE (MAX_USERNAME_LEN+10+MAX_USERNAME_LEN+12+MAX_MSG_LEN+MAX_USERNAME_LEN+10)

#define MAX_MSG_LEN 100

//for PackAnswerQuery
#define MAX_PAQ_SIZE (MAX_FRIEND_NB+10+MAX_USERNAME_LEN+12+MAX_MSG_LEN*MAX_FRIEND_NB+10+MAX_USERNAME_LEN*MAX_FRIEND_NB+10)



//the pack that contains username and password
typedef struct _PackUnamePasswd
{
    uchar action;
    char UserName[MAX_USERNAME_LEN];
    char Password[MAX_PASSWD_LEN];
} PackUnamePasswd;

//For server returning to client when they try to login or register
typedef struct _PackAnswerLR
{
    uchar successflag;//1 if PackUnamePasswd received successfully, 0 if exception occur
    int FriendNb;//The number of friends current user has
    vectorStr FriendList;//The Friends List of current user
} PackAnswerLR;

//For client to query about online status and new message or new challenges
typedef struct _PackQuery
{
     char UserName[MAX_USERNAME_LEN];//who you are
     char *NewFriend;//if you add a new friend
     char *Message;//you want to say something
     char *dstUser;//to who
} PackQuery;

//For server answering client query for online status and new message or new challenges
typedef struct _PackAnswerQuery
{
    uchar *onlineFlagList;//The online status of the friend list of current user
    char *challenger;//The list of friends that want to challenge current user
    vectorStr messageList;//the list of new messages
    vectorStr srcUserList;
} PackAnswerQuery;

//For client to chat and play between another client
typedef struct _PackPlay
{
     char UserName[MAX_USERNAME_LEN];
     uchar Action; //a hard code flag, where 1 is chat, 2 is play, 3 is undo
     char message[MAX_MSG_LEN];
     int start_pt;
     int end_pt;
} PackPlay;



//Encode Functions

void encodePackUnamePasswd(char *str_encodedPUP, PackUnamePasswd *pack);

//Decode Functions

PackUnamePasswd decodeStrPUP(char *str_encodedPUP);

#endif