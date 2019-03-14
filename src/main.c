#include"AI.h"
#include"GUI.h"
#include"util.h"
#include"constant.h"
#include"codec.h"
#include"connection.h"
#include"PlayBetween.h"

extern const char *Program;//the name of program
extern char *UserName;
extern OnlinePlayer localPlayer, remotePlayer;
extern vectorStr FriendsList;
extern bool isPlayingWithOpponent;

#define MODEL 1

//play on the current gameState by the current player
int play(GameState *gameState,Player *player,int model)
{
    int quit;
    
    if(player->identity==HUMAN) quit=gui_play(gameState,player);
    else quit=ai_play(gameState,player,model);
    return quit;
}

int onlinePlay(GameState *gameState)
{
    if(gameState->playerTurn==localPlayer.color)guio_play(gameState);
    else{
        sleep(1);//wait for the opponent to make the move
        return 0;
    }
}

//play the offline game
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
        print_stack_log(&(gameState.moves_stack));
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
    env_free_GameState(&gameState);
}

void testCodec()
{
    vectorStr friendList;
    vectorStr_init(&friendList);
    vectorStr msgList,srcUserList;
    vectorStr_init(&msgList);
    vectorStr_init(&srcUserList);
    vectorStr_add(&msgList,"hello");
    vectorStr_add(&msgList,"hello");
    vectorStr_add(&srcUserList,"keenan");
    vectorStr_add(&srcUserList,"aria");

    vectorStr_printAll(&friendList);
    vectorStr_printAll(&msgList);
    vectorStr_printAll(&srcUserList);

    PackUnamePasswd pup={LOGIN,"michaelz","25619"};
    PackAnswerLR palr={USER_LOGIN,friendList,11001};
    PackQuery pq={"michaelz","aria","hello",QUERY_CHAT,11000};
    PackAnswerQuery paq={2};
    paq.onlineFlagList[0]=paq.onlineFlagList[1]=1;
    strcpy(paq.challenger,"aria");
    paq.messageList=msgList;
    paq.srcUserList=srcUserList;
    PackPlay pp={"michaelz",PLAYBETWEEN_PLAY,"hello",48,40,QUEEN};

    char str_pup[MAX_PUP_SIZE],str_palr[MAX_PALR_SIZE],str_pq[MAX_PQ_SIZE],str_paq[MAX_PAQ_SIZE],str_pp[MAX_PP_SIZE];
    encodePackUnamePasswd(str_pup,&pup);
    encodePackAnswerLR(str_palr,&palr);
    encodePackQuery(str_pq,&pq);
    encodePackAnswerQuery(str_paq,&paq);
    encodePackPlay(str_pp,&pp);

    printf("pup:%s\n",str_pup);
    printf("palr:%s\n",str_palr);
    printf("pq:%s\n",str_pq);
    printf("paq:%s\n",str_paq);
    printf("pp:%s\n",str_pp);

    PackUnamePasswd dpup;
    PackAnswerLR dpalr;
    PackQuery dpq;
    PackAnswerQuery dpaq;
    PackPlay dpp;

    dpup=decodeStrPUP(str_pup);
    dpalr=decodeStrPALR(str_palr);
    dpq=decodeStrPQ(str_pq);
    dpaq=decodeStrPAQ(str_paq);
    dpp=decodeStrPP(str_pp);
    dpaq.friendNumber=2;

    encodePackUnamePasswd(str_pup,&dpup);
    encodePackAnswerLR(str_palr,&dpalr);
    encodePackQuery(str_pq,&dpq);
    encodePackAnswerQuery(str_paq,&dpaq);
    encodePackPlay(str_pp,&dpp);

    printf("\ntest decode\n");

    printf("dpup:%s\n",str_pup);
    printf("dpalr:%s\n",str_palr);
    printf("dpq:%s\n",str_pq);
    printf("dpaq:%s\n",str_paq);
    printf("dpp:%s\n",str_pp);

    return;
}

//a simple demo of online game(only login, and there is only one user michaelz)
void GameOnline(int argc, char *argv[])
{

    GameState gameState=env_init();
    OnlinePlayCallback onlineCallback={&gameState};

    //end of test code insertion
    int portNb=InitPlayBetweenServer(&onlineCallback);//inits the local server

    if (argc < 3)
    {
        printf("Usage: %s host_address port\n", argv[0]);
	    exit(10);
    }
    init_connection2server(argv[0],argv[1],argv[2]);

    LoginOrRegister();

    msgChat_init();//for message map
    int friendNb=vectorStr_count(&FriendsList);
    char temp[MAX_USERNAME_LEN];
    for(int i=0;i<friendNb;i++){
        msgChat_addUser(vectorStr_get(&FriendsList,i,temp),true);
    }

    init_connection2qport();

    InitQueryTimeredTask();//starts a new thread that do routine query to server

    while(true){
        
        guio_ChatsMenu();//opens the chat menu

        strcpy(localPlayer.UserName,UserName);
        guio_gameplay_window(&gameState);

        int quit=0;
        while(quit==0){
            if(!isPlayingWithOpponent)break;
            quit=onlinePlay(&gameState);
            print_stack_log(&(gameState.moves_stack));
            if(quit==CUR_PLAYER_LOSE||quit==CUR_PLAYER_WIN)
            {
                guio_refresh(&gameState);
                gui_checkmate_window(&gameState,quit);
                isPlayingWithOpponent=false;
                break;
            }
            else if(quit==QUIT)
            {
                isPlayingWithOpponent=false;
                break;
            }
            else guio_refresh(&gameState);
        }
        env_reset_GameState(&gameState);
    }
    


    env_free_GameState(&gameState);
}

//play the game
void Game(int argc, char *argv[])
{
    gui_init_window(argc,argv);

    int GameMode=gui_main_menu();
    if(GameMode==GameMode_ONLINE)GameOnline(argc,argv);
    else GameOffline();
    
}

//print the board
void print_board(GameState *gameState)
{
    for(int i=0;i<64;i++)
    {
        if(i%8==0)printf("\n");
        printf("%d\t",gameState->board[i]);
    }
    printf("\n*****************\n");
}

//output the board to a file
void fprint_board(GameState *gameState,FILE *fp)
{
    for(int i=0;i<64;i++)
    {
        if(i%8==0)printf("\n");
        fprintf(fp,"%d\t",gameState->board[i]);
    }
    fprintf(fp,"\n*****************\n");
}

//do ai contesst between different models
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
    printf("\\********************\\\n");
    #endif

    printf("\n\n\\********************\\\n");
    printf("This program will start a local server when playing with other users\n");
    printf("and will try to use a port between 11200 to 11999 of the current machine\n");
    printf("to listen to the request from other users\n");
    printf("thus please make sure there is at least one port between 11200 and 11999 is available\n");

    srand(time(0));

    Program=argv[0];


    Game(argc,argv);//play the game
    
    //AI_ContestWithGUI(argc,argv,1,2);
    
    return 0;
}
