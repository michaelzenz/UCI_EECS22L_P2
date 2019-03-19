#include"connection.h"
#include"ENV.h"
#include"pthread.h"

#ifndef PLAY_BETWEEN_H
#define PLAY_BETWEEN_H

#define PLAY_PACK_RECEIVED "Play Pack Received Successfully"
#define COLOR_SELECTION_RECEIVED "Your Color Selection Received Successfully"
#define CHALLENGE_IS_CANCELED "sorry, i cancel my challenge"
#define PLAYBETWEEN_USER_QUIT "I surrender"

//this file is for communication and play between users
//the play pack receive callback
//will be called when the a play pack is received
typedef void (*PPrecvCallback)(PackPlay pack);
//pGameState is the current gameState that the user is playing on
typedef struct _OnlinePlayCallback
{
    GameState *pGameState;
    PPrecvCallback RecvCallback;
}OnlinePlayCallback;
//the property of an online player
//there are only 2, localPlayer and remotePlayer
typedef struct _OnlinePlayer
{
    bool localUser;
    char UserName[MAX_USERNAME_LEN];
    int color;
}OnlinePlayer;

//Init the local Play Between Listener
int InitPlayBetweenServer(OnlinePlayCallback *callback);
//as is its name describes
void ShutPlayBetweenServer();
//inits the connection to opponent
void init_connection2oppo(char *host, int port);
//send a string to opponent
char* sendToOppo(char* msg);
//send a string to opponent, but will not handle the response
void SendToOppoWithoutResponse(char *msg);
//send an action to opponent, including chat, play and undo
//but chat is not yet implemented
void SendPlayAction2Oppo(uchar Action, char *msg, int start_pt,
        int end_pt, uchar promotion);

#endif