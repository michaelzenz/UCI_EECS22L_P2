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

void GameOnline(){
    printf("running game online\n");
    
    Program = argv[0];
#ifdef DEBUG
    printf("%s: Starting...\n", argv[0]);
#endif
    if (argc < 3)
    {   fprintf(stderr, "Usage: %s hostname port\n", Program);
        exit(10);
    }
    Server = gethostbyname(argv[1]);
    if (Server == NULL)
    {   fprintf(stderr, "%s: no such host named '%s'\n", Program, argv[1]);
        exit(10);
    }
    PortNo = atoi(argv[2]);
    if (PortNo <= 2000)
    {   fprintf(stderr, "%s: invalid port number %d, should be >2000\n",
                Program, PortNo);
        exit(10);
    }
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(PortNo);
    ServerAddress.sin_addr = *(struct in_addr*)Server->h_addr_list[0];
    
    /* build the GUI */
    /* (adapted from https://en.wikipedia.org/wiki/GTK%2B#Example) */
    
    /* initialize the GTK libraries */
    gtk_init(&argc, &argv);
    
    /* create the main, top level window */
    Window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    
    /* give it the title of this program */
    gtk_window_set_title(GTK_WINDOW(Window), Program);
    
    /* center the window */
    gtk_window_set_position(GTK_WINDOW(Window), GTK_WIN_POS_CENTER);
    
    /* set the window's default size */
    gtk_window_set_default_size(GTK_WINDOW(Window), 160, 280);
    gtk_container_set_border_width (GTK_CONTAINER(Window), 10);
    
    /* map the destroy signal of the window to gtk_main_quit;
     * when the window is about to be destroyed, we get a notification and
     * stop the main GTK+ loop by returning 0 */
    g_signal_connect(Window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    /* overall vertical arrangement in the window */
    VBox = gtk_vbox_new(TRUE, 10);
    gtk_container_add(GTK_CONTAINER(Window), VBox);
    
    /* on the top, put a button to request the current time from the server */
    RequestButton = gtk_button_new_with_label("Request Time from Server");
    gtk_container_add(GTK_CONTAINER(VBox), RequestButton);
    
    /* in the middle, a frame with the display of the time */
    Frame = gtk_frame_new("Time received:");
    gtk_container_add(GTK_CONTAINER(VBox), Frame);
    Label = gtk_label_new("<no time received yet>");
    gtk_container_add(GTK_CONTAINER(Frame), Label);
    
    /* on the bottom, two buttons to quit client and shutdown server */
    HButtons = gtk_hbutton_box_new();
    gtk_container_add(GTK_CONTAINER(VBox), HButtons);
    QuitButton = gtk_button_new_with_label("Quit Client");
    gtk_container_add(GTK_CONTAINER(HButtons), QuitButton);
    ShutdownButton = gtk_button_new_with_label("Shutdown Server");
    gtk_container_add(GTK_CONTAINER(HButtons), ShutdownButton);
    
    /* make sure that everything becomes visible */
    gtk_widget_show_all(Window);
    
    /* connect request button with function asking server for time */
    g_signal_connect(RequestButton, "clicked",
                     G_CALLBACK(GetTimeFromServer), Label);
    
    /* connect quit button with function terminating this client */
    /* (note the 'swapped' call; try without to see the effect) */
    g_signal_connect_swapped(QuitButton, "clicked",
                             G_CALLBACK(gtk_widget_destroy), Window);
    
    /* connect shutdown button with function terminating server and client */
    g_signal_connect(ShutdownButton, "clicked",
                     G_CALLBACK(ShutdownServer), NULL);
    
    /* start the main loop, handle all registered events */
    gtk_main();
    
    
}

void Game(int argc, char *argv[])
{
    gui_init_window(argc,argv);

    int GameMode=gui_main_menu();
    if(GameMode==GameMode_ONLINE)GameOnline();
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
    
    int PortNo;
    struct hostent *Server;
    GtkWidget *Window;
    GtkWidget *VBox, *RequestButton, *Frame, *Label;
    GtkWidget *HButtons, *QuitButton, *ShutdownButton;
    
    
    srand(time(0));
    //gui_init_window(argc,argv);
    //test_gui_menu(argc,argv);//specially created for aria to test
    // while(1)
    while(1){
        Game(argc,argv);
    }
    
    
    
    //test_env();
    //Test_AI(1);
    //AI_Contest(1,2);
    //test_gamePlay_window(argc,argv);
    //AI_ContestWithGUI(argc,argv,1,2);
    //Test_AI_withGUI(argc,argv,1);
    
    return 0;
}

/*****************/
/*functions added from GTK clock example for testing purposes*/


char *Talk2Server(
                  const char *Message,    //pointer to message you want to send
                  char *RecvBuf)          //pointer to the endpoint of the response
{
    int n;                  //used for error checkin on read and write
    int SocketFD;           //socket setting up a socket
    
    SocketFD = socket(AF_INET, SOCK_STREAM, 0);//sets up socket
    if (SocketFD < 0)
    {   FatalError("socket creation failed");
    }
#ifdef DEBUG
    printf("%s: Connecting to the server at port %d...\n",
           Program, ntohs(ServerAddress.sin_port));
#endif
    //connection is contained in if in order to return if it fails
    if (connect(SocketFD, (struct sockaddr*)&ServerAddress,
                sizeof(struct sockaddr_in)) < 0) //connects socketfd to server address, which is gloabal
    {   FatalError("connecting to server failed");
    }
    
#ifdef DEBUG
    printf("%s: Sending message '%s'...\n", Program, Message);
#endif
    //n is set to number of bytes written
    n = write(SocketFD, Message, strlen(Message));//writes message
    if (n < 0)
    {   FatalError("writing to socket failed");
    }
#ifdef DEBUG
    printf("%s: Waiting for response...\n", Program);
#endif
    n = read(SocketFD, RecvBuf, BUFFSIZE-1);//n is for the number sucessfully read, responce sent to RecBuf
    if (n < 0)
    {   FatalError("reading from socket failed");
    }
    RecvBuf[n] = 0;
#ifdef DEBUG
    printf("%s: Received response: %s\n", Program, RecvBuf);
    printf("%s: Closing the connection...\n", Program);
#endif
    close(SocketFD);
    return(RecvBuf);
} /* end of Talk2Server */

void GetTimeFromServer(        /* ask server for current time, display it */
                       GtkWidget *Widget,
                       gpointer Data)
{
    GtkWidget *ButtonClicked;
    GtkLabel *LabelToUpdate;
    char RecvBuf[BUFFSIZE];
    const char *Response;
    
#ifdef DEBUG
    printf("%s: GetTimeFromServer callback starting...\n", Program);
#endif
    ButtonClicked = Widget;
    LabelToUpdate = Data;
    assert(LabelToUpdate);
    Response = Talk2Server("TIME", RecvBuf);//
    if (0 == strncmp(Response, "OK TIME: ", 9))
    {    /* ok, strip off the protocol header and display the time */
        gtk_label_set_label(LabelToUpdate, Response + 9);
    }
    else
    {    /* unexpected, display entire response */
        gtk_label_set_label(LabelToUpdate, Response);
    }
#ifdef DEBUG
    printf("%s: GetTimeFromServer callback done.\n", Program);
#endif
} /* end of GetTimeFromServer */
