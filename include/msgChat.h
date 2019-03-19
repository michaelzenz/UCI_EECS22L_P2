#include<string.h>
#include<stdlib.h>
#include"vectorStr.h"
#include"map.h"
#include"util.h"
#include<gtk/gtk.h>

#ifndef MSG_MAP_H
#define MSG_MAP_H

//this header and it`s corresponding .c file is a local database
//for messages between different users
//as is its name describes
void msgChat_init();
//as is its name describes
//isFriend means this user is a friend or not
void msgChat_addUser(char *user, bool isFriend);
//as is its name describes
void msgChat_add_msg(char *user, char *new_msg);
//as is its name describes
void msgChat_set_isFriend(char *user, bool isFriend);
//for gui, the current pageNum in the chat page notebook
void msgChat_set_pageNum(char *user, int pageNum);
//for textview in the chat page
void msgChat_set_lastReadPos(char *user, int pos);
//to store the pointer to the buffer of the textview
void msgChat_set_buffer(char *user, GtkTextBuffer *buffer);
//as is its name describes
vectorStr* msgChat_get_msgList(char *user);
//as is its name describes
bool msgChat_get_isFriend(char* user);
//as is its name describes
bool msgChat_get_isUserExist(char* user);
//as is its name describes
int msgChat_get_pageNum(char* user);
//get the iter of the corresponding buffer
GtkTextIter* msgChat_get_iter(char* user);
//as is its name describes
int msgChat_get_lastReadPos(char *user);
//get the buffer of the textview
GtkTextBuffer* msgChat_get_buffer(char *user);

#endif