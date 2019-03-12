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

extern bool isPlayingWithOpponent;

//for chat menu
GtkListStore *guioFriendListStore;
GtkListStore *guioUnknownListStore;
GtkWidget *guioFriendTreeView;
GtkWidget *guioUnknownTreeView;
GtkCellRenderer *guioFriendTreeRenderer;
GtkTreeViewColumn *FriendListColumn;
GtkTreeViewColumn *UnknownListColumn;
GtkWidget *guioFriendTreeScroll;
GtkWidget *guioUnknownTreeScroll;

int guioFriendTreePageNum,guioUnknownTreePageNum;

GtkWidget *FriendTreeNotebook;
GtkWidget *NoteBookFixed;
GtkWidget *ChatMenuScroll;
GtkWidget *ChatPageBook;
GtkWidget *MsgTextView;
GtkWidget *SendButton;
GtkWidget *RMpageButton;
GtkWidget *ChallengeButton;

bool ChatMenuInitialized=false;
//!for chat menu

//the pixbuf to load image and resize from a .jpg or .png file
GdkPixbuf *Login_pixbuf = NULL;
GdkPixbuf *Register_pixbuf = NULL;
GdkPixbuf *Chats_pixbuf = NULL;
GdkPixbuf *Add_friends_pixbuf = NULL;
GdkPixbuf *OnlinePlay_pixbuf=NULL;//for board

extern GtkWidget *chess_icon;//the icon to draw on the board

char *Login_menu_path="res/Login.png";
char *Register_menu_path="res/Register_Menu.png";
char *Chats_menu_path="res/background.png";
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
    dialog = gtk_message_dialog_new (NULL,
                                 GTK_DIALOG_DESTROY_WITH_PARENT,
                                 GTK_MESSAGE_ERROR,
                                 GTK_BUTTONS_CLOSE,
                                 "%s",
                                 msg);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}


//Only ask yes and no
bool guio_AskQuestion(char *msg)
{
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new (NULL,
                                 GTK_DIALOG_DESTROY_WITH_PARENT,
                                 GTK_MESSAGE_OTHER,
                                 GTK_BUTTONS_YES_NO,
                                 "%s",
                                 msg);
    bool ret=gtk_dialog_run (GTK_DIALOG (dialog))==GTK_RESPONSE_YES;
    gtk_widget_destroy (dialog);
    return ret;
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

gint Chats_menu_callback (GtkWidget *widget, GdkEvent  *event, gpointer data)
{
    int x, y;
    GdkModifierType state;
    gdk_window_get_pointer(widget->window,&x,&y,&state);
    printf("x:%d, y:%d\n",x,y);
}


/* Remove a page from the notebook */
void remove_book (GtkNotebook *notebook)
{
    gint page;
    page = gtk_notebook_current_page(notebook);

    char* selectedUserName=gtk_notebook_get_tab_label_text(notebook,
        gtk_notebook_get_nth_page(notebook,page));

    msgChat_set_pageNum(selectedUserName,-1);
    msgChat_set_lastReadPos(selectedUserName,-1);
    msgChat_set_buffer(selectedUserName,NULL);

    gtk_notebook_remove_page (notebook, page);
    /* Need to refresh the widget -- 
     This forces the widget to redraw itself. */
    gtk_widget_draw(GTK_WIDGET(notebook), NULL);
    guio_onMsgUpdate();
}

//WARNING: the gchar* return by this func must be free
gchar* guio_getUserSelection(GtkTreeModel *model, GtkTreeView *tree){
    GtkTreeIter iter;
    gchar* user;
    GtkTreeSelection *selection=gtk_tree_view_get_selection (tree);
    if (gtk_tree_selection_get_selected (selection, &model, &iter))
    {
        gtk_tree_model_get(model, &iter, 0, &user, -1);
    }
    return user;
}

void guio_CHAT_send_msg()
{
    int page=gtk_notebook_current_page(GTK_NOTEBOOK(ChatPageBook));
    if(page<0){
        guio_ErrorMsg("please specify a user to send message!");
        return;
    }
    char* selectedUser=gtk_notebook_get_tab_label_text(GTK_NOTEBOOK(ChatPageBook),
        gtk_notebook_get_nth_page(GTK_NOTEBOOK(ChatPageBook),
        page));

    GtkTextIter start, end;
    GtkTextBuffer *textBuf=gtk_text_view_get_buffer(GTK_TEXT_VIEW(MsgTextView));
    gtk_text_buffer_get_bounds(textBuf, &start, &end);
    char *msg = gtk_text_buffer_get_text(textBuf, &start, &end, TRUE);
    if(sizeof(msg)>MAX_MSG_LEN){
        printf("This message is too long, please limit to MAX_MSG_LEN\n");
        return;
    }
    gtk_text_buffer_set_text(textBuf,"",-1);
    sprintf(msg,"%s",msg);
    msgChat_add_msg(selectedUser,msg);
    guio_onMsgUpdate();
    SendMsgToUser(selectedUser,msg);
}

void guio_addfriend(char *UserName)
{
    GtkTreeIter   iter;
    gtk_list_store_append(guioFriendListStore, &iter);
    gtk_list_store_set(guioFriendListStore,&iter, 0, UserName,-1);
}

void guio_addUnkown(char *UserName)
{
    GtkTreeIter   iter;
    gtk_list_store_append(guioUnknownListStore, &iter);
    gtk_list_store_set(guioUnknownListStore, &iter, 0, UserName,-1);
}

gint guio_openChatPage(GtkTreeView *treeview, GtkTreePath *path,
    GtkTreeViewColumn *col, gpointer userdata)
{
    char *selectedUser;
    if(gtk_notebook_get_current_page(GTK_NOTEBOOK(FriendTreeNotebook))==guioFriendTreePageNum)
        selectedUser=guio_getUserSelection(GTK_TREE_MODEL(guioFriendListStore),
            GTK_TREE_VIEW(guioFriendTreeView));
    else
        selectedUser=guio_getUserSelection(GTK_TREE_MODEL(guioUnknownListStore),
            GTK_TREE_VIEW(guioUnknownTreeView));
        
    if(msgChat_get_pageNum(selectedUser)>=0){
        gtk_notebook_set_current_page(GTK_NOTEBOOK(ChatPageBook),msgChat_get_pageNum(selectedUser));
        return 0;
    }

    GtkWidget *label;
    char bufferl[32];
    sprintf(bufferl, "%s", selectedUser);

    GtkWidget *newChatPage=gtk_text_view_new();
    gtk_widget_set_size_request (newChatPage, CHAT_PAGE_WIDTH, CHAT_PAGE_HEIGHT);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(newChatPage),false);
    gtk_widget_show(newChatPage);
    GtkWidget *newScroll=gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(newScroll, CHAT_PAGE_WIDTH, CHAT_PAGE_HEIGHT);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (newScroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
    gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (newScroll), newChatPage);

    gtk_widget_show(newScroll);

    label = gtk_label_new (bufferl);
    int pageNum=gtk_notebook_append_page (GTK_NOTEBOOK(ChatPageBook), newScroll, label);
    msgChat_set_pageNum(selectedUser,pageNum);
    GtkTextBuffer *buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(newChatPage));
    gtk_text_buffer_get_iter_at_offset(buffer, msgChat_get_iter(selectedUser), 0);
    msgChat_set_buffer(selectedUser,buffer);
    free(selectedUser);
    guio_onMsgUpdate();
    return 0;
}

int _addWidgetToFriendListNotebook(GtkWidget *list, char *listName)
{
    GtkWidget *label;
    char bufferl[32];
    sprintf(bufferl,"%s",listName);
    label = gtk_label_new (bufferl);
    int pageNum=gtk_notebook_prepend_page (GTK_NOTEBOOK(FriendTreeNotebook), list, label);
    return pageNum;
}

void guio_challenge()
{
    gchar *selectedUser;
        if(gtk_notebook_get_current_page(GTK_NOTEBOOK(FriendTreeNotebook))==guioFriendTreePageNum)
        selectedUser=guio_getUserSelection(GTK_TREE_MODEL(guioFriendListStore),
            GTK_TREE_VIEW(guioFriendTreeView));
    else
        selectedUser=guio_getUserSelection(GTK_TREE_MODEL(guioUnknownListStore),
            GTK_TREE_VIEW(guioUnknownTreeView));
    ChallengeUser(selectedUser);
    strcpy(remotePlayer.UserName,selectedUser);
    free(selectedUser);
    isPlayingWithOpponent=true;
}

void InitChatMenu()
{
    gdk_threads_enter();
    
    guioFriendListStore = gtk_list_store_new (1, G_TYPE_STRING);
    guioUnknownListStore = gtk_list_store_new (1, G_TYPE_STRING);
    guioFriendTreeView=gtk_tree_view_new_with_model(GTK_TREE_MODEL(guioFriendListStore));
    guioUnknownTreeView=gtk_tree_view_new_with_model(GTK_TREE_MODEL(guioUnknownListStore));
    g_object_unref (G_OBJECT(guioFriendListStore));
    g_object_unref (G_OBJECT(guioUnknownListStore));
    GtkTreeSelection  *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(guioFriendTreeView));
    gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(guioUnknownTreeView));
    gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);
    g_signal_connect(guioFriendTreeView, "row-activated", (GCallback)guio_openChatPage, NULL);
    g_signal_connect(guioUnknownTreeView, "row-activated", (GCallback)guio_openChatPage, NULL);

    guioFriendTreeRenderer = gtk_cell_renderer_text_new ();
    g_object_set (G_OBJECT (guioFriendTreeRenderer), "foreground", "orange", NULL);

    FriendListColumn=gtk_tree_view_column_new_with_attributes ("", guioFriendTreeRenderer,
         "text", 0, NULL);
    
    gtk_tree_view_append_column (GTK_TREE_VIEW (guioFriendTreeView), FriendListColumn);
    guioFriendTreeRenderer = gtk_cell_renderer_text_new ();
    g_object_set (G_OBJECT (guioFriendTreeRenderer), "foreground", "orange", NULL);

    UnknownListColumn=gtk_tree_view_column_new_with_attributes ("", guioFriendTreeRenderer,
         "text", 0, NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (guioUnknownTreeView), UnknownListColumn);
    
    
    guioFriendTreeScroll=gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(guioFriendTreeScroll, -1, CHAT_LIST_HEIGHT);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (guioFriendTreeScroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
    gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (guioFriendTreeScroll), guioFriendTreeView);

    guioUnknownTreeScroll=gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(guioUnknownTreeScroll, -1, FRIEND_LIST_WIDTH);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (guioUnknownTreeScroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
    gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (guioUnknownTreeScroll), guioUnknownTreeView);

    FriendTreeNotebook=gtk_notebook_new();
    gtk_notebook_set_tab_pos (GTK_NOTEBOOK (FriendTreeNotebook), GTK_POS_TOP);
    gtk_widget_set_size_request (FriendTreeNotebook, FRIEND_LIST_WIDTH, CHAT_LIST_HEIGHT);

    guioUnknownTreePageNum=_addWidgetToFriendListNotebook(guioUnknownTreeScroll,"Unknown");
    guioFriendTreePageNum=_addWidgetToFriendListNotebook(guioFriendTreeScroll,"Friends");

    ChatMenuScroll = gtk_scrolled_window_new (NULL,NULL);
    gtk_container_border_width (GTK_CONTAINER (ChatMenuScroll), 10);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (ChatMenuScroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
    gtk_widget_set_usize(ChatMenuScroll, CHAT_SCROLL_WIDTH, CHAT_LIST_HEIGHT);

    ChatPageBook = gtk_notebook_new ();
    gtk_notebook_set_tab_pos (GTK_NOTEBOOK (ChatPageBook), GTK_POS_TOP);
    gtk_widget_set_size_request (ChatPageBook, NOTEBOOK_FIXED_WIDTH, NOTEBOOK_FIXED_HEIGHT);
    NoteBookFixed=gtk_fixed_new();
    gtk_fixed_put(GTK_FIXED(NoteBookFixed), ChatPageBook, NOTEBOOK_FIXED_LEFT, NOTEBOOK_FIXED_TOP);

    MsgTextView=gtk_text_view_new();
    gtk_widget_set_size_request (MsgTextView, MSG_TEXTVIEW_FIXED_WIDTH, MSG_TEXTVIEW_FIXED_HEIGHT);
    gtk_fixed_put(GTK_FIXED(NoteBookFixed), MsgTextView, MSG_TEXTVIEW_FIXED_LEFT, MSG_TEXTVIEW_FIXED_TOP);

    gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (ChatMenuScroll), NoteBookFixed);

    SendButton = gtk_button_new_with_label ("send");
    gtk_widget_set_size_request (SendButton, CHAT_BUTTON_WIDTH, CHAT_BUTTON_HEIGHT);
    gtk_signal_connect_object (GTK_OBJECT (SendButton), "clicked",
                            (GtkSignalFunc) guio_CHAT_send_msg,
                            NULL);

    RMpageButton = gtk_button_new_with_label ("remove page");
    gtk_widget_set_size_request (RMpageButton, CHAT_BUTTON_WIDTH, CHAT_BUTTON_HEIGHT);
    gtk_signal_connect_object (GTK_OBJECT (RMpageButton), "clicked",
                            (GtkSignalFunc) remove_book,
                            ChatPageBook);

    ChallengeButton = gtk_button_new_with_label ("Challenge");
    gtk_widget_set_size_request (ChallengeButton, CHALLENGE_BUTTON_WIDTH, CHALLENGE_BUTTON_HEIGHT);
    gtk_signal_connect_object (GTK_OBJECT (ChallengeButton), "clicked",
                            (GtkSignalFunc) guio_challenge,
                            NULL);

    gtk_fixed_put(GTK_FIXED(NoteBookFixed), SendButton, CHAT_BUTTON_LEFT, SEND_BUTTON_TOP);
    gtk_fixed_put(GTK_FIXED(NoteBookFixed), RMpageButton, CHAT_BUTTON_LEFT, RMPAGE_BUTTON_TOP);

    gdk_threads_leave();
}

void guio_onMsgUpdate()
{
    gint page;
    page = gtk_notebook_current_page(GTK_NOTEBOOK(ChatPageBook));
    if(page<0)return;

    char* selectedUserName=gtk_notebook_get_tab_label_text(GTK_NOTEBOOK(ChatPageBook),
        gtk_notebook_get_nth_page(GTK_NOTEBOOK(ChatPageBook),page));

    int pos=msgChat_get_lastReadPos(selectedUserName);
    vectorStr *msgList=msgChat_get_msgList(selectedUserName);
    int msgCnt=vectorStr_count(msgList);
    if(pos<msgCnt-1){
        GtkTextIter *iter=msgChat_get_iter(selectedUserName);
        GtkTextBuffer *buffer=msgChat_get_buffer(selectedUserName);
        char temp[MAX_MSG_LEN];
        for(int i=pos+1;i<msgCnt;i++){
            gtk_text_buffer_insert(buffer,iter,vectorStr_get(msgList,i,temp),-1);
            gtk_text_buffer_insert(buffer,iter,"\n",-1);
        }
        msgChat_set_lastReadPos(selectedUserName,msgCnt-1);
    }
}

void Chats_menu()
{
    if(!ChatMenuInitialized){
        InitChatMenu();
        ChatMenuInitialized=true;
    }

    gdk_threads_enter();//this is important, before you call any gtk_* or g_* or gdk_* functions, call this function first

    int friendNb=vectorStr_count(&FriendsList);
    char temp[MAX_USERNAME_LEN];
    for(int i=0;i<friendNb;i++){
        guio_addfriend(vectorStr_get(&FriendsList,i,temp));
    }

    Chats_pixbuf=load_pixbuf_from_file(Chats_menu_path);  //loads the background image from files
    Chats_pixbuf=gdk_pixbuf_scale_simple(Chats_pixbuf,WINDOW_WIDTH,WINDOW_HEIGHT,GDK_INTERP_BILINEAR);  //sets bg image to size of window
    image = gtk_image_new_from_pixbuf(Chats_pixbuf);  //sets variable for bg image
    gtk_layout_put(GTK_LAYOUT(layout), image, 0, 0);  //add bg image to layout
    gtk_layout_put(GTK_LAYOUT(layout), FriendTreeNotebook, FRIEND_LIST_LEFT, FRIEND_LIST_TOP);
    gtk_layout_put(GTK_LAYOUT(layout), ChatMenuScroll, 20, 40);
    gtk_layout_put(GTK_LAYOUT(layout), ChallengeButton, CHALLENGE_BUTTON_LEFT, CHALLENGE_BUTTON_TOP);

    gulong handlerID=g_signal_connect(window, "button_press_event", G_CALLBACK(Chats_menu_callback),NULL);  //connect signals from clicking the window to active the callback

    gtk_widget_show_all(window);   //shows the window to the user
    gdk_threads_leave();//after you finich calling gtk functions, call this
    while(!isPlayingWithOpponent){
        sleep(1);
    }
    
    gdk_threads_enter();//again, you know what I am gonna say
    g_signal_handler_disconnect(window,handlerID);
    gdk_threads_leave();
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
    if(pixelX>=842&&pixelX<=940&&pixelY>=66&&pixelY<=95)
    {
        check_ActionMade=ACTION_UNDO;
        return;
    }
    if(pixelX>=852&&pixelX<=931&&pixelY>=117&&pixelY<=149)
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
    else if(check_ActionMade==ACTION_QUIT)
        check= ACTION_QUIT;
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
    gtk_container_remove(GTK_CONTAINER(layout),FriendTreeNotebook);
    gtk_container_remove(GTK_CONTAINER(layout),ChatMenuScroll);
    gtk_container_remove(GTK_CONTAINER(layout),ChallengeButton);
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