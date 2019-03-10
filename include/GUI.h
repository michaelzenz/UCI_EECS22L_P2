
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

#ifndef GAMEGUI_H
#define GAMEGUI_H

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

//for online
#define NOT_YET_LOGIN 0
#define LOGIN_SUCCESS 1
#define NOT_YET_REGISTER 2
#define GO_TO_REGISTER 3
#define REGISTER_SUCCESS 4
//!for online

//init a new window
GdkPixbuf *load_pixbuf_from_file (const char *filename);

void gui_init_window(int argc, char*argv[]);

//draws the game menu and return GameMode from user
int gui_main_menu();

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
void Chats_menu();
/*********************/
//!Online
/*********************/

#endif
