#include"GUI.h"

extern int PlayBetweenServerPort;
extern vectorStr FriendsList;
extern OnlinePlayer remotePlayer;

extern GtkWidget *window;
extern GtkWidget *image;
extern GtkWidget *layout;//the layout to put on background and contain fixed widget
extern GtkWidget *fixed;//the widget to contain table
extern GtkWidget *table;//the widget to contain icons

GtkWidget *usernameEntry;
GtkWidget *passwordEntry, *verifyPasswordEntry;

bool isLoginRegisterInitialized=false;

//the pixbuf to load image and resize from a .jpg or .png file
GdkPixbuf *Login_pixbuf = NULL;
GdkPixbuf *Register_pixbuf = NULL;
GdkPixbuf *OnlinePlay_pixbuf=NULL;//for board

extern GtkWidget *chess_icon;//the icon to draw on the board

char *Login_menu_path="res/Login.png";
char *Register_menu_path="res/Register_Menu.png";

char *OnlinePlay_Background="res/OnlineChess_background.png";

uchar LoginFlag=NOT_YET_LOGIN;

extern char *UserName;
extern int QueryPort;

//Look up table
extern char *str_square[4];
extern char *str_color[2];
extern char *str_piece[7];



//add friend to friend list 
void guio_addfriend(char *UserName);
void guio_onMsgUpdate();

void empty_container(GtkWidget *container)
{
    GList *children, *iter;
    //testing the destroy stuff in a container
    children = gtk_container_get_children(GTK_CONTAINER(container));
    for(iter = children; iter != NULL; iter = g_list_next(iter))
    gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);
    //end test
}

void guio_ErrorMsg(char *msg)
{
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new (NULL, GTK_DIALOG_DESTROY_WITH_PARENT,
                                 GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
                                 "%s", msg);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

void guio_InformMsg(char *msg)
{
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new (NULL, GTK_DIALOG_DESTROY_WITH_PARENT,
                                 GTK_MESSAGE_OTHER, GTK_BUTTONS_CLOSE,
                                 "%s", msg);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

//Only ask yes and no
bool guio_AskQuestion(char *msg)
{
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new (NULL, GTK_DIALOG_DESTROY_WITH_PARENT,
                                 GTK_MESSAGE_OTHER, GTK_BUTTONS_YES_NO,
                                 "%s", msg);
    bool ret=gtk_dialog_run (GTK_DIALOG (dialog))==GTK_RESPONSE_YES;
    gtk_widget_destroy (dialog);
    return ret;
}

GtkWidget *waitActionDialog=NULL;
WaitUserActionCallback waitUserActionCallback;

void waitUserActionResponseHandler (void* pdata)
{
    waitUserActionCallback(pdata);
    gtk_widget_destroy(waitActionDialog);
}

void guio_waitUserActionWithCallback(char *msg, WaitUserActionCallback callback, void *pdata)
{
    waitUserActionCallback=callback;
    waitActionDialog = gtk_message_dialog_new (NULL,
                                 GTK_DIALOG_DESTROY_WITH_PARENT,
                                 GTK_MESSAGE_OTHER,
                                 GTK_BUTTONS_CANCEL,
                                 "%s",
                                 msg);
    g_signal_connect_swapped (waitActionDialog, "response",
                          G_CALLBACK (waitUserActionResponseHandler),
                          pdata);
    //gtk_container_add(GTK_CONTAINER(waitActionDialogWindow), waitActionDialog);
    gtk_widget_show(waitActionDialog);
}

void guio_waitUserAction(char *msg)
{
    waitActionDialog = gtk_message_dialog_new (NULL,
                                 GTK_DIALOG_DESTROY_WITH_PARENT,
                                 GTK_MESSAGE_OTHER,
                                 GTK_BUTTONS_CANCEL,
                                 "%s",
                                 msg);
    g_signal_connect_swapped (waitActionDialog, "response",
                          G_CALLBACK (gtk_widget_destroy),
                          waitActionDialog);
    //gtk_container_add(GTK_CONTAINER(waitActionDialogWindow), waitActionDialog);
    gtk_widget_show(waitActionDialog);
}

void guio_removeWaitActionDialog()
{
    if(waitActionDialog!=NULL)
        gtk_widget_destroy(waitActionDialog);
}

void initLoginRegister(){
    Login_pixbuf=load_pixbuf_from_file(Login_menu_path);
    Login_pixbuf=gdk_pixbuf_scale_simple(Login_pixbuf,WINDOW_WIDTH,WINDOW_HEIGHT,GDK_INTERP_BILINEAR);
    Register_pixbuf=load_pixbuf_from_file(Register_menu_path); //loads the background image from files
    Register_pixbuf=gdk_pixbuf_scale_simple(Register_pixbuf,WINDOW_WIDTH,WINDOW_HEIGHT,GDK_INTERP_BILINEAR);//sets bg image to size of window

    usernameEntry = gtk_entry_new();
    passwordEntry= gtk_entry_new();
    verifyPasswordEntry= gtk_entry_new();  // creates a text entry for verify password
    gtk_entry_set_visibility(GTK_ENTRY(passwordEntry), FALSE);
    gtk_entry_set_invisible_char (GTK_ENTRY (passwordEntry), '*');
    gtk_entry_set_visibility(GTK_ENTRY(verifyPasswordEntry), FALSE);  //makes text entered not be readable
    gtk_entry_set_invisible_char (GTK_ENTRY (verifyPasswordEntry), '*');  //replaces letters with *
}

gint Login_menu_callback (GtkWidget *widget, GdkEvent  *event, gpointer data)
{
    int x, y;
    GdkModifierType state;
    gdk_window_get_pointer(widget->window,&x,&y,&state);
    printf("x:%d, y:%d\n",x,y);

    if(x>636&&x<724&&y>302&&y<333)
    {   
        char Sendstr[MAX_PUP_SIZE];
        printf("logingin\n");
        char Username[MAX_USERNAME_LEN];
        char Passwd[MAX_PASSWD_LEN];

        sprintf(Username,"%s",gtk_entry_get_text(GTK_ENTRY(usernameEntry)));
        sprintf(Passwd,"%s",gtk_entry_get_text(GTK_ENTRY(passwordEntry)));
        if(strlen(Username)==0||strlen(Passwd)==0){
            guio_ErrorMsg("PLEASE INPUT USERNAME/PASSWORD");
            return 0;
        }
        PackUnamePasswd packUP;
        packUP.action=LOGIN;
        strcpy(packUP.UserName,Username);
        strcpy(packUP.Password,Passwd); 
        encodePackUnamePasswd(Sendstr,&packUP);
        printf("%s\n",Sendstr);
        
        char *RecvBuf=sendToServer(Sendstr);
        PackAnswerLR palr=decodeStrPALR(RecvBuf);
        if(palr.successflag==USER_LOGIN){
            if(UserName==NULL)UserName=malloc(strlen(Username)+1);
            strcpy(UserName,Username);//this is for the global one
            LoginFlag=LOGIN_SUCCESS;
            QueryPort=palr.QueryPort;
            FriendsList=palr.FriendList;
        }
        else if(palr.successflag==INVALID_PASSWD){
            guio_ErrorMsg("Invalid Password, Please Try Agagin!");
        }
        else if(palr.successflag==NO_SUCH_USER){
            guio_ErrorMsg("No Such User, Please Register First!");
        }
	    free(RecvBuf);
        printf("Trying to login\n");
    }
    else if(x>613&&x<747&&y>350&&y<380)
    {
        printf("Register");
        guio_waitUserAction("test");
        LoginFlag=NOT_YET_REGISTER;
    }
}

int Login_menu()
{
    if(!isLoginRegisterInitialized){
        initLoginRegister();
        isLoginRegisterInitialized=true;
    }
    gdk_threads_enter();//this is important, before you call any gtk_* or g_* or gdk_* functions, call this function first
    image = gtk_image_new_from_pixbuf(Login_pixbuf);
    gtk_layout_put(GTK_LAYOUT(layout), image, 0, 0);
    gtk_layout_put(GTK_LAYOUT(layout), usernameEntry, U_P_VP_LEFT, USERNAME_TOP );
    gtk_layout_put(GTK_LAYOUT(layout), passwordEntry, U_P_VP_LEFT, PASSWD_TOP );

    gulong handlerID=g_signal_connect(window, "button_press_event", G_CALLBACK(Login_menu_callback),NULL);
    gtk_widget_show_all(window);
    gdk_threads_leave();//after you finich calling gtk functions, call this
    while(LoginFlag==NOT_YET_LOGIN)sleep(1);//must call sleep to release some cpu resources for gtk thread to run
    gdk_threads_enter();//again, you know what I am gonna say
    g_signal_handler_disconnect(window,handlerID);
    g_object_ref(usernameEntry);
    g_object_ref(passwordEntry);
    gtk_container_remove(GTK_CONTAINER(layout),usernameEntry);
    gtk_container_remove(GTK_CONTAINER(layout),passwordEntry);
    gdk_threads_leave();
    return LoginFlag;
}


gint Register_menu_callback (GtkWidget *widget, GdkEvent  *event, gpointer data)
{
    int x, y;
    GdkModifierType state;
    gdk_window_get_pointer(widget->window,&x,&y,&state);
    GtkWidget *label;
    printf("x:%d, y:%d\n",x,y);
    if(x>622&&x<741&&y>351&&y<377)
    {
        char Sendstr[MAX_PUP_SIZE];
        printf("Registering\n");
        char Username[MAX_USERNAME_LEN];
      	char Passwd[MAX_PASSWD_LEN];
	    char verifyPasswd[MAX_PASSWD_LEN];
        sprintf(Username,"%s",gtk_entry_get_text(GTK_ENTRY(usernameEntry)));
        sprintf(Passwd,"%s",gtk_entry_get_text(GTK_ENTRY(passwordEntry)));
	    sprintf(verifyPasswd,"%s",gtk_entry_get_text(GTK_ENTRY(verifyPasswordEntry)));
        
        if(strcmp(verifyPasswd,Passwd)){
            guio_ErrorMsg("PASSWORDS DOESN`T MATCH, PLEASE TRY AGAIN");
            return 0;
        }
        if(strlen(Username)==0||strlen(Passwd)==0||strlen(verifyPasswd)==0){
            guio_ErrorMsg("PLEASE INPUT USERNAME/PASSWORD/VERIFY_PASSWORD");
            return 0;
        }
        PackUnamePasswd packUP;
        packUP.action=REGISTER;
        strcpy(packUP.UserName,Username);
        strcpy(packUP.Password,Passwd); 
        encodePackUnamePasswd(Sendstr,&packUP);
        printf("%s\n",Sendstr);
        
        char *RecvBuf=sendToServer(Sendstr);
        PackAnswerLR palr=decodeStrPALR(RecvBuf);
        if(palr.successflag==USER_REGISTER){
            LoginFlag=REGISTER_SUCCESS;
            if(UserName==NULL)UserName=malloc(strlen(Username)+1);
            strcpy(UserName,Username);//this is for the global one
            QueryPort=palr.QueryPort;
            FriendsList=palr.FriendList;
	        printf("you have been successfully registered");
        }
        else if(palr.successflag==USER_ALREADY_EXIST){
            guio_ErrorMsg("User already exist, please login");
        }
	    free(RecvBuf);
        printf("Trying to Register\n");
    }
    else if(x>133&&x<202&&y>456&&y<478){
        LoginFlag=NOT_YET_LOGIN;
    }
}

int Register_menu()
{
    gdk_threads_enter();//this is important, before you call any gtk_* or g_* or gdk_* functions, call this function first

    image = gtk_image_new_from_pixbuf(Register_pixbuf); //sets variable for bg image
    gtk_layout_put(GTK_LAYOUT(layout), image, 0, 0);  //add bg image to layout

    gtk_layout_put(GTK_LAYOUT(layout), usernameEntry, U_P_VP_LEFT, USERNAME_TOP );
    gtk_layout_put(GTK_LAYOUT(layout), passwordEntry, U_P_VP_LEFT, PASSWD_TOP );
    gtk_layout_put(GTK_LAYOUT(layout), verifyPasswordEntry, U_P_VP_LEFT, VERIFY_PASSWD_TOP );
    
    gulong handlerID=g_signal_connect(window, "button_press_event", G_CALLBACK(Register_menu_callback),NULL); //connect signals from clicking the window to active the callback
    gtk_widget_show_all(window); //shows the window to the user
    gdk_threads_leave();//after you finich calling gtk functions, call this
    while(LoginFlag==NOT_YET_REGISTER)sleep(1);//must call sleep to release some cpu resources for gtk thread to run
    gdk_threads_enter();//again, you know what I am gonna say
    g_signal_handler_disconnect(window,handlerID); //disconnects the signals from clicking
    g_object_ref(usernameEntry);
    g_object_ref(passwordEntry);
    g_object_ref(verifyPasswordEntry);
    gtk_container_remove(GTK_CONTAINER(layout),verifyPasswordEntry);
    gtk_container_remove(GTK_CONTAINER(layout),usernameEntry);
    gtk_container_remove(GTK_CONTAINER(layout),passwordEntry);
    gdk_threads_leave();
    return LoginFlag;
}

void LoginOrRegister()
{
    while(LoginFlag!=LOGIN_SUCCESS&&LoginFlag!=REGISTER_SUCCESS){
        if(LoginFlag==NOT_YET_LOGIN)Login_menu();
        if(LoginFlag==NOT_YET_REGISTER)Register_menu();
    }
}



extern int check_ActionMade;//1 is normal end, 2 is undo
extern int check_legal_start;//selecting a movable piece
extern int move_start;//start point
extern int move_end;//end poing
extern vector cur_legal_moves;//legal moves from current starting position

//Draws and refresh the board
void guio_DrawBoard(GameState *gamestate,int start_pt,vector legal_moves)
{
    table = gtk_table_new (8, 8, TRUE) ;
    gtk_widget_set_size_request (table, BOARD_WIDTH, BOARD_HEIGHT);

	int x, y;
	char path[50];
	for(int i = 0 ; i< 64; i++)
    {
        memset(path,'\0',sizeof(path));
        x = (i)%8;
        y = (i)/8;
        
        if(vector_contain(&legal_moves,i))strcat(path,str_square[3]);
        else if(i==start_pt)strcat(path,str_square[2]);
        else strcat(path,str_square[(x+y)%2]);

        if(gamestate->board[i]==BLANK)strcat(path,str_piece[BLANK]);
        else
        {
            int color=gamestate->board[i]/abs(gamestate->board[i]);
            int colorID=MAX(color*-1,0);
            strcat(path, str_color[colorID]);
            strcat(path,str_piece[abs(gamestate->board[i])]);
        }

        chess_icon=gtk_image_new_from_file(path);
        gtk_table_attach(GTK_TABLE(table), chess_icon, x, x+1, y, y+1, GTK_FILL, GTK_FILL, 0, 0);
    }

    fixed = gtk_fixed_new();
    gtk_fixed_put(GTK_FIXED(fixed), table, ONLINE_BOARD_LEFT, ONLINE_BOARD_UP);
    gtk_container_add(GTK_CONTAINER(layout), fixed);
    gtk_widget_show_all(window);
}

//window coordinates to grid coordinates
void guio_CoordToGrid(int c_x, int c_y, int *g_x, int *g_y)
{
        *g_x = (c_x - ONLINE_BOARD_LEFT) / SQUARE_SIZE;
        *g_y = (c_y - ONLINE_BOARD_UP) / SQUARE_SIZE;
}

//callback for gui play
void guio_play_callback(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    int pixelX, pixelY, gridX, gridY, index, piece;
    GameState *gameState=(GameState*)data;
	

 	GdkModifierType state;
	
	//gets the location of where the person clicked
	gdk_window_get_pointer(widget->window, &pixelX, &pixelY, &state);


    printf("pX: %d, pY: %d\n",pixelX,pixelY);
    if(pixelX>=821&&pixelX<=894&&pixelY>=474&&pixelY<=498)
    {
        check_ActionMade=ACTION_UNDO;
        return;
    }
    if(pixelX>=64&&pixelX<=132&&pixelY>=476&&pixelY<=497)
    {
        check_ActionMade=ACTION_QUIT;
        return;
    }
    if(pixelX<=BOARD_BORDER_LEFT||pixelX>=BOARD_BORDER_RIGHT||
        pixelY<=BOARD_BORDER_UP||pixelY>=BOARD_BORDER_DOWN)return;
	//change pixel to xy coordinates
	guio_CoordToGrid(pixelX, pixelY, &gridX, &gridY);
    printf("gX:%d, gY:%d\n",gridX,gridY);
    int pos=gridY*8+gridX;

    
    if(!check_legal_start)
    {
        int move_vector_cnt=gameState->moves_vector_cnt;
        
        for(int i=0;i<move_vector_cnt;i++)
        {
            if(pos==gameState->container[i].pos)
            {
                cur_legal_moves=gameState->container[i].legal_moves;
                check_legal_start=1;
                move_start=pos;
                break;
            }
        }
        if(check_legal_start)
        {
            gtk_container_remove(GTK_CONTAINER(layout), fixed);
            guio_DrawBoard(gameState,pos,cur_legal_moves);
        }
        else
        {
            vector empty;
            vector_init(&empty);
            gtk_container_remove(GTK_CONTAINER(layout), fixed);
            guio_DrawBoard(gameState,-1,empty);
        }
        
    }
    else 
    {
        if(vector_contain(&cur_legal_moves,pos))
        {
            move_end=pos;
            check_ActionMade=ACTION_PLAY;
        }
        else
        {
            check_legal_start=0;
            move_end=-1;
            move_start=-1;
        }
    }
    
    
}

//called if a human is the turn to play
//connect click signal to window and draw the board based on the user`s action
int guio_play(GameState *gameState)
{
    Player dummyPlayer;
    int check=env_check_end(gameState,&dummyPlayer);
    if(check!=0)
    {
        env_free_container(gameState);
        return check;
    }
	gdk_threads_enter();
    gulong handlerID=g_signal_connect(window, "button_press_event", G_CALLBACK(guio_play_callback), gameState);
    gdk_threads_leave();
    while(check_ActionMade==0){
        sleep(1);
    }
    gdk_threads_enter();
    g_signal_handler_disconnect(window,handlerID);
    gdk_threads_leave();

    
    if(check_ActionMade==ACTION_PLAY){
        env_play2(gameState,move_start,move_end,QUEEN);
        SendPlayAction2Oppo(PLAYBETWEEN_PLAY,"",move_start,move_end,QUEEN);
    }
    else if(check_ActionMade==ACTION_UNDO)
    {
        env_undo(gameState);
        env_undo(gameState);
        SendPlayAction2Oppo(PLAYBETWEEN_UNDO,"",move_start,move_end,BLANK);
    }
    else if(check_ActionMade==ACTION_QUIT){
        check= ACTION_QUIT;
        SendToOppoWithoutResponse(PLAYBETWEEN_USER_QUIT);
    }
    move_start=-1;
    move_start=-1;
    check_legal_start=0;
    check_ActionMade=0;
    env_free_container(gameState);


    return check ;
}

//draws the game play window
void guio_gameplay_window(GameState *gameState)
{
	/*create a table and draw the board*/
    gdk_threads_enter();//this is important, before you call any gtk_* or g_* or gdk_* functions, call this function first
    // empty_container(window);
    
    OnlinePlay_pixbuf=load_pixbuf_from_file(OnlinePlay_Background);
    OnlinePlay_pixbuf=gdk_pixbuf_scale_simple(OnlinePlay_pixbuf,WINDOW_WIDTH,WINDOW_HEIGHT,GDK_INTERP_BILINEAR);
    image = gtk_image_new_from_pixbuf(OnlinePlay_pixbuf);
    gtk_layout_put(GTK_LAYOUT(layout), image, 0, 0);
    vector empty;
    vector_init(&empty);
    guio_DrawBoard(gameState,-1,empty);
    gdk_threads_leave();

    //when mouse presses window callback (TBD)
  	//g_signal_connect(window, "button_press_event", G_CALLBACK( TBD ), NULL) ;
}

void guio_refresh(GameState *gameState)
{
    gdk_threads_enter();//this is important, before you call any gtk_* or g_* or gdk_* functions, call this function first
 
    gtk_container_remove(GTK_CONTAINER(layout), fixed) ; 
    
    vector empty;
    vector_init(&empty);
    guio_DrawBoard(gameState,-1,empty);

    gdk_threads_leave();
}