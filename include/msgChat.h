#include<string.h>
#include<stdlib.h>
#include"vectorStr.h"
#include"map.h"
#include"util.h"

#ifndef MSG_MAP_H
#define MSG_MAP_H

void msgChat_init();

void msgChat_addUser(char *user, bool isFriend);

void msgChat_add_msg(char *user, char *new_msg);

void msgChat_set_isFriend(char *user, bool isFriend);

void msgChat_set_pageNum(char *user, int pageNum);

vectorStr* msgChat_get_msgList(char *user);

bool msgChat_get_isFriend(char* user);

bool msgChat_get_isUserExist(char* user);

int msgChat_get_pageNum(char* user);

#endif