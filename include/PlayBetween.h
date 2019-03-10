#include"connection.h"
#include"ENV.h"
#include"pthread.h"

#ifndef PLAY_BETWEEN_H
#define PLAY_BETWEEN_H

typedef void (*PPrecvCallback)(PackPlay pack);

typedef struct _OnlinePlayCallback
{
    GameState *pGameState;
    PPrecvCallback RecvCallback;
}OnlinePlayCallback;

typedef struct _OnlinePlayer
{
    char serverAddress[20];
    char port[20];
    int color;
    bool localUser;
}OnlinePlayer;

//Init the local Play Between Listener
int InitPlayBetweenServer(OnlinePlayCallback *callback);
void ShutPlayBetweenServer();

#endif