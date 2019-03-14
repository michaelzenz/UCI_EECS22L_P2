
#include"util.h"
#include<string.h>
#include"jsmn.h"
#include<stdlib.h>
#include"vectorStr.h"

#ifndef CODEC_H
#define CODEC_H


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

//success flags
#define USER_LOGIN 0
#define NO_SUCH_USER 1
#define INVALID_PASSWD 2
#define USER_REGISTER 3
#define USER_ALREADY_EXIST 4

//for PackQuery
#define MAX_PQ_SIZE (MAX_USERNAME_LEN+10+MAX_USERNAME_LEN+12+MAX_MSG_LEN+MAX_USERNAME_LEN+10)

#define MAX_MSG_LEN 50

//query actions
#define QUERY_CHAT 0
#define QUERY_CHALLENGE 1
#define QUERY_ADD_FRIEND 2
#define QUERY_CHECK_UPDATE 3
#define QUERY_DELETE_FRIEND 4

//for PackAnswerQuery
#define MAX_PAQ_SIZE (MAX_FRIEND_NB+10+MAX_USERNAME_LEN+12+MAX_MSG_LEN*MAX_FRIEND_NB+10+MAX_USERNAME_LEN*MAX_FRIEND_NB+10)

#define ADD_FRIENDS_SUCCESSFULLY -2
#define FRIENDS_ALREADY_EXIST -3
#define DELETE_FRIENDS_SUCCESSFULLY -4
#define USER_NOT_YOUR_FRIENDS -5

//for PackPlay
#define MAX_PP_SIZE (MAX_MSG_LEN+50)

#define PLAYBETWEEN_CHAT 1
#define PLAYBETWEEN_PLAY 2
#define PLAYBETWEEN_UNDO 3

#define MAX_JSON_OBJ_LEN 50

//the pack that contains username and password
typedef struct _PackUnamePasswd
{
    uchar action;//login or register
    char UserName[MAX_USERNAME_LEN];
    char Password[MAX_PASSWD_LEN];
    int port;//the port that this user uses to listen to other users
} PackUnamePasswd;

//For server returning to client when they try to login or register
typedef struct _PackAnswerLR
{
    uchar successflag;//please see success flags in for PackAnswerLR part
    vectorStr FriendList;//The Friends List of current user
    int QueryPort;//The port of the server that allows user to do query
} PackAnswerLR;

//For client to query about online status and new message or new challenges
//you can only do chat or challenge at one time
typedef struct _PackQuery
{
    char UserName[MAX_USERNAME_LEN];//who you are
    //the user that will be challenged, chat or add to friends
    //depends on the action
    char dstUser[MAX_USERNAME_LEN];
    char Message[MAX_MSG_LEN];//if you want to say something
    uchar action;//please see query actions part above (see codec.h for list of actions)
    int portNb;//the local host port
} PackQuery;

//For server answering client query for online status and new message or new challenges
typedef struct _PackAnswerQuery
{
    int friendNumber;//this will not be pack into the encoded string, but is necessary for encoding
    uchar onlineFlagList[MAX_FRIEND_NB];//The online status of the friend list of current user
    //The user that want to challenge current user
    //important: the challenger can also be the user, which means the
    //current user is the challenger
    char challenger[MAX_USERNAME_LEN];
    char opponentHost[20];//the opponent`s host
    int opponentPort;//the opponent`s port
    vectorStr messageList;//the list of new messages
    vectorStr srcUserList;//from who
} PackAnswerQuery;

//For client to chat and play between another client
typedef struct _PackPlay
{
     char UserName[MAX_USERNAME_LEN];//the User that made this action
     uchar Action; //a hard code flag, where 1 is chat, 2 is play, 3 is undo
     char message[MAX_MSG_LEN];//msg to current opponent
     int start_pt;
     int end_pt;
     uchar promotion;//the promotion that the user selects
} PackPlay;

typedef struct _PackSearch
{
    char *targetUserName;
}PackSearch;


typedef struct _PackAnswerSearch
{
    vectorStr MatchList;
}PackAnswerSearch;

//Encode Functions

void encodePackUnamePasswd(char *str_encodedPUP, PackUnamePasswd *pack);
void encodePackAnswerLR(char *jsonStr, PackAnswerLR *pack);
void encodePackQuery(char *jsonStr, PackQuery *pack);
void encodePackAnswerQuery(char *jsonStr, PackAnswerQuery *pack);
void encodePackPlay(char *jsonStr, PackPlay *pack);
char* encodePackSearch(PackSearch *pack);
char* encodePackAnswerSearch(PackAnswerSearch *pack);

//Decode Functions

PackUnamePasswd decodeStrPUP(char *str_encodedPUP);
PackAnswerLR decodeStrPALR(char *jsonStr);
PackQuery decodeStrPQ(char *jsonStr);
PackAnswerQuery decodeStrPAQ(char *jsonStr);
PackPlay decodeStrPP(char *jsonStr);
PackSearch decodePS(char *jsonStr);
PackAnswerSearch decodeStrPAS(char *jsonStr);

#endif