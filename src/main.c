#include"AI.h"
#include"GUI.h"
#include"struct.h"
#include"constant.h"
#include"codec.h"
#include"connection.h"

#define MODEL 1


int play(GameState *gameState,Player *player,int model)
{
    int quit;
    
    if(player->identity==HUMAN) quit=gui_play(gameState,player);
    else quit=ai_play(gameState,player,model);
    return quit;
}

void GameOffline(){
    Player player_arr[2];
    GameState gameState=env_init();
    gui_init_offline(&gameState,player_arr);
    Player player1,player2;
    player1=player_arr[0];
    player2=player_arr[1];
    player1.id=PLAYER1;
    player2.id=PLAYER2;

    int quit=0;
    while(quit==0)
    {
        int quit;
        if(gameState.playerTurn==player1.color)
            quit=play(&gameState,&player1,player1.difficulty);
        else 
            quit=play(&gameState,&player2,player2.difficulty);
        stack_print_log(&(gameState.moves_stack));
        if(quit==CUR_PLAYER_LOSE||quit==CUR_PLAYER_WIN)
        {
            gui_refresh(&gameState,player_arr);
            gui_checkmate_window(&gameState,quit);
            return;
        }
        else if(quit==QUIT)
        {
            return;
        }
        else gui_refresh(&gameState,player_arr);
    }
}

void GameOnline(int argc, char *argv[]){
    Register_menu();
    sleep(3);
    Login_menu();
    sleep(3);

    if (argc < 3)
    {   
        printf("Usage: %s hostname port\n", argv[0]);
	    exit(10);
    }
    init_connection2server(argv[0],argv[1],argv[2]);
    
    printf("running game online\n");
    printf("\ntry to login: michaelz, 25619\n");
    PackUnamePasswd up={"michaelz","25619"};
    char str_up[MAX_UP_SIZE];
    memset(str_up,'\0',sizeof(str_up));
    encodePackUnamePasswd(str_up,&up);
    printf("%s\n",str_up);//just for test decode
    up=decodeStrUP(str_up);//just for test decode
    char RecvBuf[BUFFERSIZE]; 
    sendToServer(str_up,RecvBuf);

    printf("\ntry to login: michaelz, 25618\n");
    strcpy(up.Password,"25618");
    encodePackUnamePasswd(str_up,&up);
    sendToServer(str_up,RecvBuf);

    printf("\ntry to login: keenan, 25618\n");
    strcpy(up.UserName,"keenan");
    encodePackUnamePasswd(str_up,&up);
    sendToServer(str_up,RecvBuf);
    int hit=1;
    while(1==1)sleep(1);
}

void Game(int argc, char *argv[])
{
    gui_init_window(argc,argv);

    int GameMode=gui_main_menu();
    if(GameMode==GameMode_ONLINE)GameOnline(argc,argv);
    else GameOffline();
    
}

void print_board(GameState *gameState)
{
    for(int i=0;i<64;i++)
    {
        if(i%8==0)printf("\n");
        printf("%d\t",gameState->board[i]);
    }
    printf("\n*****************\n");
}

void fprint_board(GameState *gameState,FILE *fp)
{
    for(int i=0;i<64;i++)
    {
        if(i%8==0)printf("\n");
        fprintf(fp,"%d\t",gameState->board[i]);
    }
    fprintf(fp,"\n*****************\n");
}

void AI_ContestWithGUI(int argc, char *argv[],int model1,int model2)
{
    gui_init_window(argc,argv);
    GameState gameState=env_init();
    Player player1,player2;
    player1.color=WHITE;
    player2.color=BLACK;
    player1.id=0;
    player2.id=1;
    player1.identity=player2.identity=COMPUTER;
    gui_gameplay_window(&gameState);

    int quit=0;
    while(quit==0)
    {
        if(gameState.playerTurn==player1.color)quit=ai_play(&gameState,&player1,model1);
        else quit=ai_play(&gameState,&player2,model2);
        // sleep(1);
        //print_board(&gameState);
        gui_refresh(&gameState,&player1);
    }
}


int main(int argc, char *argv[])
{   
    #ifdef SUPEROLD
    printf("\n\n\\********************\\\n");
    printf("You are seeing this message because you are using a very old version of Glib\n");
    printf("This is strongly NOT recommended because when TODAY`s HUMAN BEINGS download you project from Internet\n");
    printf("You expect them to install the latest glib instead of some outdated version\n");
    printf("But after all we can`t do anything to stop you.\n");
    printf("Even if you may not really want to\n");
    #endif
    srand(time(0));
    //gui_init_window(argc,argv);
    //test_gui_menu(argc,argv);//specially created for aria to test
    // while(1)
    Game(argc,argv);
    
    
    
    //test_env();
    //Test_AI(1);
    //AI_Contest(1,2);
    //test_gamePlay_window(argc,argv);
    //AI_ContestWithGUI(argc,argv,1,2);
    //Test_AI_withGUI(argc,argv,1);
    
    return 0;
}