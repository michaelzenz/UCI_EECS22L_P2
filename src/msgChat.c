
#include"msgChat.h"

map_void_t msgChat;

typedef struct _ChatInfo
{
    vectorStr msgList;
    bool isFriend;
    int pageNum;//this is for guio, the page number in the notebook
}ChatInfo;

void msgChat_init()
{
    map_init(&msgChat);
}

ChatInfo* _newChatInfo(bool isFriend)
{
    ChatInfo *newInfo=malloc(sizeof(ChatInfo));
    vectorStr_init(&newInfo->msgList);
    newInfo->isFriend=isFriend;
    newInfo->pageNum=-1;
}

void msgChat_addUser(char *user, bool isFriend)
{
    ChatInfo *newInfo=_newChatInfo(isFriend);
    map_set(&msgChat,user,newInfo);
}

void msgChat_add_msg(char *user, char *new_msg)
{
    ChatInfo *newInfo=map_get(&msgChat,user);
    vectorStr_add(&newInfo->msgList,new_msg);
}

void msgChat_set_isFriend(char *user, bool isFriend)
{
    ChatInfo *info=map_get(&msgChat,user);
    info->isFriend=isFriend;
}

void msgChat_set_pageNum(char *user, int pageNum)
{
    ChatInfo *info=map_get(&msgChat,user);
    info->pageNum=pageNum;
}

vectorStr* msgChat_get_msgList(char *user)
{
    ChatInfo *info=map_get(&msgChat,user);
    return &info->msgList;
}

bool msgChat_get_isFriend(char* user)
{
    ChatInfo *info=map_get(&msgChat,user);
    return info->isFriend;
}

bool msgChat_get_isUserExist(char* user)
{
    ChatInfo *info=map_get(&msgChat,user);
    return info!=NULL;
}

int msgChat_get_pageNum(char* user)
{
    ChatInfo *info=map_get(&msgChat,user);
    return info->pageNum;
}