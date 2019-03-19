
#include"ENV.h"
#include<string.h>
#include"util.h"
#include"constant.h"
#include"stack.h"
#include <gtk/gtk.h>
#include <glib.h>
#include <stdio.h>
#include "codec.h"
#include "connection.h"
#include "PlayBetween.h"
#include"msgChat.h"

#ifndef GAMEGUI_H
#define GAMEGUI_H

//for offline
#define MAX_MSGLEN  100
#define SQUARE_SIZE 50
#define WINDOW_BORDER 0
#define BOARD_WIDTH  (8*SQUARE_SIZE)
#define BOARD_HEIGHT (8*SQUARE_SIZE)
#define BOARD_BORDER_UP (WINDOW_HEIGHT/2-BOARD_HEIGHT/2)
#define BOARD_BORDER_LEFT (WINDOW_WIDTH/2-BOARD_WIDTH/2)
#define BOARD_BORDER_DOWN (WINDOW_HEIGHT/2+BOARD_HEIGHT/2)
#define BOARD_BORDER_RIGHT (WINDOW_WIDTH/2+BOARD_WIDTH/2)
#define WINDOW_WIDTH  960
#define WINDOW_HEIGHT 540

#define GameMode_HvC 1
#define GameMode_HvH 2
#define GameMode_CvC 3
#define GameMode_ONLINE 4

#define ACTION_PLAY 1
#define ACTION_UNDO 2
#define ACTION_QUIT 3
//!for offline

//for online gui properties

#define U_P_VP_LEFT 380 //the left of username, passwd and vpasswd
#define USERNAME_TOP 168
#define PASSWD_TOP 219
#define VERIFY_PASSWD_TOP 270


//LoginFlags
#define NOT_YET_LOGIN 0
#define LOGIN_SUCCESS 1
#define NOT_YET_REGISTER 2
#define REGISTER_SUCCESS 3

#define CHAT_SCROLL_WIDTH 725

#define CHAT_LIST_HEIGHT 450

#define FRIEND_LIST_LEFT 750
#define FRIEND_LIST_TOP 40
#define FRIEND_LIST_WIDTH 200

#define NOTEBOOK_FIXED_WIDTH 690
#define NOTEBOOK_FIXED_HEIGHT 340
#define NOTEBOOK_FIXED_LEFT 10
#define NOTEBOOK_FIXED_TOP 10

#define CHAT_PAGE_WIDTH 100
#define CHAT_PAGE_HEIGHT 260

#define MSG_TEXTVIEW_FIXED_WIDTH 490
#define MSG_TEXTVIEW_FIXED_HEIGHT 60
#define MSG_TEXTVIEW_FIXED_LEFT 10
#define MSG_TEXTVIEW_FIXED_TOP 360

#define CHAT_BUTTON_WIDTH 150
#define CHAT_BUTTON_HEIGHT 20
#define CHAT_BUTTON_LEFT 550
#define SEND_BUTTON_TOP 360
#define RMPAGE_BUTTON_TOP 390

#define CHALLENGE_BUTTON_WIDTH 120
#define CHALLENGE_BUTTON_HEIGHT 30
#define CHALLENGE_BUTTON_LEFT 790
#define CHALLENGE_BUTTON_TOP (FRIEND_LIST_TOP+CHAT_LIST_HEIGHT)


#define ADDFRIEND_BUTTON_LEFT 650
#define ADDFRIEND_BUTTON_TOP (FRIEND_LIST_TOP+CHAT_LIST_HEIGHT)

#define DELETEFRIEND_BUTTON_LEFT 510
#define DELETEFRIEND_BUTTON_TOP (FRIEND_LIST_TOP+CHAT_LIST_HEIGHT)

#define ONLINE_BOARD_UP (WINDOW_HEIGHT/2-BOARD_HEIGHT/2)
#define ONLINE_BOARD_LEFT (WINDOW_WIDTH/2-BOARD_WIDTH/2)
#define ONLINE_BOARD_DOWN (WINDOW_HEIGHT/2+BOARD_HEIGHT/2)
#define ONLINE_BOARD_RIGHT (WINDOW_WIDTH/2+BOARD_WIDTH/2)

//!for online

//as is its name describes
GdkPixbuf *_load_pixbuf_from_file (const char *filename);

//init a new window and a gtk thread
void gui_init_window(int argc, char*argv[]);

//draws the game menu and return GameMode from user
int gui_main_menu();

//the following funcs start with guio_ is for external use
//the following creates different dialogs 
//only show an error msg
void guio_ErrorMsg(char *msg);
//only show an msg
void guio_InformMsg(char *msg);
//ask a question, returns true or false according to the user`s click
bool guio_AskQuestion(char *question);

void guio_waitUserAction(char *msg);

//there is noly one button in this dialog, the callback will be called if the user clicks it
typedef void(*WaitUserActionCallback)(void*);
void guio_waitUserActionWithCallback(char *msg, WaitUserActionCallback callback, void *pdata);
//call this to remove the dialog created by the function above
void guio_removeWaitActionDialog();

//the following funcs start with _ is for GUI*.c internal use
//as is its name describes
//same as above, but for internal use
void _ErrorMsg(char *msg);
void _InformMsg(char *msg);
bool _AskQuestion(char *question);

/*********************/
//Offline
/*********************/
//init the offline players
void gui_init_offline(GameState *gameState,Player player_arr[2]);

//let human play on the gameState
int gui_play(GameState *gameState,Player *player);

//refresh the gui window
void gui_refresh(GameState *gameState,Player *player_arr);

//show the checkmate window if one of the player wins(not yet finish)
void gui_checkmate_window(GameState *gameState, int winner);

//draws the game play window(that contains the board)
void gui_gameplay_window(GameState *gameState);

/*********************/
//!Offline
/*********************/

/*********************/
//Online
/*********************/
//Try to login or register
void LoginOrRegister();

//Opens the chat menu
void guio_ChatsMenu();
//renders the gameplay window
void guio_gameplay_window(GameState *gameState);
//refresh the board
void guio_refresh(GameState *gameState);
//let the user play on board
int guio_play(GameState *gameState);
//will be called when new message arrive
void guio_onMsgUpdate();
//add a friend to treeview
void guio_addfriend2Tree(char *UserName);
//as is its name describes
//same as above
void guio_addUnkown2Tree(char *UserName);
//will be called when a friend is added
void guio_onFriendAdded();

void _createAddFriendWinCallback();
/*********************/
//!Online
/*********************/

#endif
