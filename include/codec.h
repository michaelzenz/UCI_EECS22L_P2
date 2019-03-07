
#include"struct.h"
#include<string.h>
#include"jsmn.h"
#include<stdlib.h>
#include"vectorStr.h"

#ifndef CODEC_H
#define CODEC_H

#define MAX_UP_SIZE 250

#define MAX_USERNAME_LEN 30
#define MAX_PASSWD_LEN 30

//for PackUnamePasswd
#define LOGIN 0
#define REGISTER 1

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
    char *FriendsList[MAX_USERNAME_LEN];//The Friends List of current user
} PackAnswerLR;


//Encode Functions

void encodePackUnamePasswd(char *str_encodedPUP, PackUnamePasswd *pack);

//Decode Functions

PackUnamePasswd decodeStrUP(char *str_encodedPUP);

#endif