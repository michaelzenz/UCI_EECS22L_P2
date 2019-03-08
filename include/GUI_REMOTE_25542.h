
#include"ENV.h"
#include<string.h>
#include"struct.h"
#include"constant.h"
#include"stack.h"
#include <gtk/gtk.h>
#include <glib.h>
#include <stdio.h>


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

//init a new window
void gui_init_window(int argc, char*argv[]);

//init the offline players
void gui_init_offline(GameState *gameState,Player player_arr[2]);

//draws the game menu and return GameMode from user
int gui_main_menu();

//let human play on the gameState
int gui_play(GameState *gameState,Player *player);

//refresh the gui window
void gui_refresh(GameState *gameState,Player *player_arr);

//show the checkmate window if one of the player wins(not yet finish)
void gui_checkmate_window(GameState *gameState, int winner);

//draws the game play window(that contains the board)
void gui_gameplay_window(GameState *gameState);

#endif
