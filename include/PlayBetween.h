#include"connection.h"
#include"ENV.h"
#include"pthread.h"

#ifndef PLAY_BETWEEN_H
#define PLAY_BETWEEN_H

#define PLAY_PACK_RECEIVED "Play Pack Received Successfully"

typedef void (*PPrecvCallback)(PackPlay pack);

typedef struct _OnlinePlayCallback
{
    GameState *pGameState;
    PPrecvCallback RecvCallback;
}OnlinePlayCallback;

typedef struct _OnlinePlayer
{
    bool localUser;
    char UserName[MAX_USERNAME_LEN];
    int color;
}OnlinePlayer;

//Init the local Play Between Listener
int InitPlayBetweenServer(OnlinePlayCallback *callback);

void ShutPlayBetweenServer();

void init_connection2oppo(char *host, int port);

void SendPlayAction2Oppo(uchar Action, char *msg, int start_pt,
        int end_pt, uchar promotion);

#endif