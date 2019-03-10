
#include"util.h"
#include<string.h>
#include"jsmn.h"
#include<stdlib.h>
#include"vectorStr.h"

#ifndef CODEC_H
#define CODEC_H

#define MAX_JSON_OBJ_LEN 50
#define MAX_JSMN_TOKEN_NB 128

#define MAX_USERNAME_LEN 30
#define MAX_PASSWD_LEN 30

//for PackUnamePasswd

#define MAX_PUP_SIZE 250

#define LOGIN 0
#define REGISTER 1

//for PackAnswerLR
#define MAX_PALR_SIZE (10+10+MAX_FRIEND_NB*MAX_USERNAME_LEN+10)

#define MAX_FRIEND_NB 2

#define USER_LOGIN 0
#define NO_SUCH_USER 1
#define INVALID_PASSWD 2
#define USER_REGISTER 3

//for PackQuery
#define MAX_PQ_SIZE (MAX_USERNAME_LEN+10+MAX_USERNAME_LEN+12+MAX_MSG_LEN+MAX_USERNAME_LEN+10)

#define MAX_MSG_LEN 100

//for PackAnswerQuery
#define MAX_PAQ_SIZE (MAX_FRIEND_NB+10+MAX_USERNAME_LEN+12+MAX_MSG_LEN*MAX_FRIEND_NB+10+MAX_USERNAME_LEN*MAX_FRIEND_NB+10)

//for PackPlay
#define MAX_PP_SIZE (MAX_MSG_LEN+50)

#define CHAT 1
#define PLAY 2
#define UNDO 3

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
    vectorStr FriendList;//The Friends List of current user
} PackAnswerLR;

//For client to query about online status and new message or new challenges
typedef struct _PackQuery
{
     char UserName[MAX_USERNAME_LEN];//who you are
     char NewFriend[MAX_USERNAME_LEN];//if you add a new friend
     char Message[MAX_MSG_LEN];//you want to say something
     char dstUser[MAX_USERNAME_LEN];//to who
     int portNb;
} PackQuery;

//For server answering client query for online status and new message or new challenges
typedef struct _PackAnswerQuery
{
    int friendNumber;//this will not be pack into the encoded string, but is necessary for encoding
    uchar onlineFlagList[MAX_FRIEND_NB];//The online status of the friend list of current user
    char challenger[MAX_USERNAME_LEN];//The list of friends that want to challenge current user
    vectorStr messageList;//the list of new messages
    vectorStr srcUserList;
    int QueryPort;
} PackAnswerQuery;

//For client to chat and play between another client
typedef struct _PackPlay
{
     char UserName[MAX_USERNAME_LEN];
     uchar Action; //a hard code flag, where 1 is chat, 2 is play, 3 is undo
     char message[MAX_MSG_LEN];
     int start_pt;
     int end_pt;
     uchar promotion;
} PackPlay;



//Encode Functions

void encodePackUnamePasswd(char *str_encodedPUP, PackUnamePasswd *pack);
void encodePackAnswerLR(char *jsonStr, PackAnswerLR *pack);
void encodePackQuery(char *jsonStr, PackQuery *pack);
void encodePackAnswerQuery(char *jsonStr, PackAnswerQuery *pack);
void encodePackPlay(char *jsonStr, PackPlay *pack);


//Decode Functions

PackUnamePasswd decodeStrPUP(char *str_encodedPUP);
PackAnswerLR decodeStrPALR(char *jsonStr);
PackQuery decodeStrPQ(char *jsonStr);
PackAnswerQuery decodeStrPAQ(char *jsonStr);
PackPlay decodeStrPP(char *jsonStr);

#endif