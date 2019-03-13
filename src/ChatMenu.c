
#include"GUI.h"

extern int PlayBetweenServerPort;
extern vectorStr FriendsList;
extern OnlinePlayer remotePlayer;
GtkWidget *friendwindow;
extern GtkWidget *window;
extern GtkWidget *image;
extern GtkWidget *layout;//the layout to put on background and contain fixed widget
GtkWidget *Friendentry;

//for chat menu
extern bool isPlayingWithOpponent;

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
GtkWidget *AddFriendButton;
GtkWidget *DeleteFriendButton;
GtkWidget *layout2;

bool ChatMenuInitialized=false;
//!for chat menu

char *Chats_menu_path="res/background.png";
GdkPixbuf *Chats_pixbuf = NULL;
GdkPixbuf *Add_friends_pixbuf = NULL;


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
    GtkWidget *addbutton;
    
    gtk_list_store_append(guioFriendListStore, &iter);
    gtk_list_store_set(guioFriendListStore,&iter, 0, UserName,-1);
    friendwindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(friendwindow), "Add Friend");
    gtk_window_set_default_size(GTK_WINDOW(friendwindow), 320, 200);
    layout2=gtk_fixed_new();
     Friendentry=gtk_entry_new();
    addbutton = gtk_button_new_with_label("Add");
    gtk_fixed_put(layout2,addbutton,150,100);
    gtk_container_add(GTK_CONTAINER(friendwindow), layout2);

   gtk_container_add(GTK_CONTAINER(friendwindow), Friendentry);

   gtk_widget_show_all(friendwindow);
}
void guio(char *UserName)
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

    AddFriendButton = gtk_button_new_with_label ("Add Friend");
    gtk_widget_set_size_request (AddFriendButton, CHALLENGE_BUTTON_WIDTH, CHALLENGE_BUTTON_HEIGHT);
    gtk_signal_connect_object (GTK_OBJECT (AddFriendButton), "clicked",
                            (GtkSignalFunc) guio_addfriend,
                            NULL);

    DeleteFriendButton = gtk_button_new_with_label ("Delete Friend");
    gtk_widget_set_size_request (DeleteFriendButton, CHALLENGE_BUTTON_WIDTH, CHALLENGE_BUTTON_HEIGHT);
    gtk_signal_connect_object (GTK_OBJECT (DeleteFriendButton), "clicked",
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
        int friendNb=vectorStr_count(&FriendsList);
        char temp[MAX_USERNAME_LEN];
        for(int i=0;i<friendNb;i++){
            guio_addfriend(vectorStr_get(&FriendsList,i,temp));
        }
        ChatMenuInitialized=true;
    }

    gdk_threads_enter();//this is important, before you call any gtk_* or g_* or gdk_* functions, call this function first
    Chats_pixbuf=load_pixbuf_from_file(Chats_menu_path);  //loads the background image from files
    Chats_pixbuf=gdk_pixbuf_scale_simple(Chats_pixbuf,WINDOW_WIDTH,WINDOW_HEIGHT,GDK_INTERP_BILINEAR);  //sets bg image to size of window
    image = gtk_image_new_from_pixbuf(Chats_pixbuf);  //sets variable for bg image
    gtk_layout_put(GTK_LAYOUT(layout), image, 0, 0);  //add bg image to layout
    gtk_layout_put(GTK_LAYOUT(layout), FriendTreeNotebook, FRIEND_LIST_LEFT, FRIEND_LIST_TOP);
    gtk_layout_put(GTK_LAYOUT(layout), ChatMenuScroll, 20, 40);
    gtk_layout_put(GTK_LAYOUT(layout), ChallengeButton, CHALLENGE_BUTTON_LEFT, CHALLENGE_BUTTON_TOP);
    gtk_layout_put(GTK_LAYOUT(layout), AddFriendButton, ADDFRIEND_BUTTON_LEFT, ADDFRIEND_BUTTON_TOP);
    gtk_layout_put(GTK_LAYOUT(layout), DeleteFriendButton, DELETEFRIEND_BUTTON_LEFT, DELETEFRIEND_BUTTON_TOP);


    gulong handlerID=g_signal_connect(window, "button_press_event", G_CALLBACK(Chats_menu_callback),NULL);  //connect signals from clicking the window to active the callback

    gtk_widget_show_all(window);   //shows the window to the user
    gdk_threads_leave();//after you finich calling gtk functions, call this
    while(!isPlayingWithOpponent){
        sleep(1);
    }
    
    gdk_threads_enter();//again, you know what I am gonna say
    g_object_ref(FriendTreeNotebook);
    g_object_ref(ChatMenuScroll);
    g_object_ref(ChallengeButton);
    g_object_ref(AddFriendButton);
    g_object_ref(DeleteFriendButton);
    gtk_container_remove(GTK_CONTAINER(layout),FriendTreeNotebook);
    gtk_container_remove(GTK_CONTAINER(layout),ChatMenuScroll);
    gtk_container_remove(GTK_CONTAINER(layout),ChallengeButton);
    gtk_container_remove(GTK_CONTAINER(layout),AddFriendButton);
    gtk_container_remove(GTK_CONTAINER(layout),DeleteFriendButton);
    g_signal_handler_disconnect(window,handlerID);
    gdk_threads_leave();
}