#include"GUI.h"

extern GtkWidget *window;
extern GtkWidget *image;
extern GtkWidget *layout;//the layout to put on background and contain fixed widget
extern GtkWidget *fixed;//the widget to contain table
extern GtkWidget *table;//the widget to contain icons
extern GtkWidget *text_view; // widget to write text into log

GtkWidget *username;
GtkWidget *password, *verifyPassword;
GtkWidget *tableU, *tableP, *tableVP;
GtkWidget *scrolled_win;
GtkWidget *friendsList;
GtkTextBuffer *buffer=NULL;
GList *children, *iter;

//the pixbuf to load image and resize from a .jpg or .png file
GdkPixbuf *Login_pixbuf = NULL;
GdkPixbuf *Register_pixbuf = NULL;
GdkPixbuf *Chats_pixbuf = NULL;
GdkPixbuf *Add_friends_pixbuf = NULL;

char *Login_menu_path="res/Login.png";
char *Register_menu_path="res/Register_Menu.png";
char *Chats_menu_path="res/background.png";

uchar LoginFlag=NOT_YET_LOGIN;
bool isUserExist=true;

extern char *UserName;

void empty_container(GtkWidget *container)
{
    //testing the destroy stuff in a container
    children = gtk_container_get_children(GTK_CONTAINER(container));
    for(iter = children; iter != NULL; iter = g_list_next(iter))
    gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);
    //end test
}

gint Login_menu_callback (GtkWidget *widget, GdkEvent  *event, gpointer data)
{
    int x, y;
    GdkModifierType state;
    gdk_window_get_pointer(widget->window,&x,&y,&state);
    
    printf("x:%d, y:%d\n",x,y);
   
    //gtk_text_buffer_set_text (buffer, "Your 1st GtkTextView widget!", -1);

    if(x>636&&x<724&&y>302&&y<333)
    {   
        char Sendstr[MAX_PUP_SIZE];
        printf("logingin\n");
        char Username[MAX_USERNAME_LEN];
        
        char Passwd[MAX_PASSWD_LEN];

        sprintf(Username,"%s",gtk_entry_get_text(username));
        sprintf(Passwd,"%s",gtk_entry_get_text(password));
        if(strlen(Username)==0||strlen(Passwd)==0)return 0;
        PackUnamePasswd packUP;
        packUP.action=LOGIN;
        strcpy(packUP.UserName,Username);
        strcpy(packUP.Password,Passwd); 
        encodePackUnamePasswd(Sendstr,&packUP);
        if(UserName==NULL)UserName=malloc(strlen(Username)+1);
        strcpy(UserName,Username);//this is for the global one
        
        char *RecvBuf=sendToServer(Sendstr);
        PackAnswerLR palr=decodeStrPALR(RecvBuf);
        if(palr.successflag==USER_LOGIN)LoginFlag=LOGIN_SUCCESS;
        else if(palr.successflag==INVALID_PASSWD)LoginFlag=NOT_YET_LOGIN;
        else if(palr.successflag==NO_SUCH_USER){
            isUserExist=false;
            LoginFlag=NOT_YET_REGISTER;
        }
	    free(RecvBuf);
        printf("Trying to login\n");
    }
    else if(x>613&&x<747&&y>350&&y<380)
    {
        printf("Register");
        if(!isUserExist)LoginFlag=NOT_YET_REGISTER;
    }
    else if(x>124&&x<211&&y>452&&y<482)
    {
        printf("BACK");
    }
}

int Login_menu()
{
    
    gdk_threads_enter();//this is important, before you call any gtk_* or g_* or gdk_* functions, call this function first
    Login_pixbuf=load_pixbuf_from_file(Login_menu_path);
    Login_pixbuf=gdk_pixbuf_scale_simple(Login_pixbuf,WINDOW_WIDTH,WINDOW_HEIGHT,GDK_INTERP_BILINEAR);
    tableU=gtk_table_new (10, 10, TRUE);
    tableP=gtk_table_new (10, 10, TRUE);
    image = gtk_image_new_from_pixbuf(Login_pixbuf);
    username = gtk_entry_new();
    password= gtk_entry_new();
   // text_view=gtk_entry_get_text (buffer);

    //
//  gtk_container_add (GTK_CONTAINER (scrolled_win), text_view);
//     gtk_container_set_border_width (GTK_CONTAINER (scrolled_win), 300);
//     gtk_container_add (GTK_CONTAINER (layout), scrolled_win);
    gtk_table_attach (GTK_TABLE (tableU), username, 0, 2, 0, 4,
        GTK_EXPAND, GTK_SHRINK, 380, 168);
    gtk_table_attach (GTK_TABLE (tableP), password, 0, 2, 0, 4,
        GTK_EXPAND, GTK_SHRINK, 380, 219);
    gtk_layout_put(GTK_LAYOUT(layout), image, 0, 0);
    gtk_container_add (GTK_CONTAINER (layout), tableU);
    gtk_container_add (GTK_CONTAINER (layout), tableP);
    gtk_entry_set_visibility(GTK_ENTRY(password), FALSE);
    gtk_entry_set_invisible_char (GTK_ENTRY (password), '*');
    
    gulong handlerID=g_signal_connect(window, "button_press_event", G_CALLBACK(Login_menu_callback),NULL);
    gtk_widget_show_all(window);
    gdk_threads_leave();//after you finich calling gtk functions, call this
    while(LoginFlag==NOT_YET_LOGIN)sleep(1);//must call sleep to release some cpu resources for gtk thread to run
    gdk_threads_enter();//again, you know what I am gonna say
    g_signal_handler_disconnect(window,handlerID);
    gdk_threads_leave();
    return LoginFlag;
}


gint Register_menu_callback (GtkWidget *widget, GdkEvent  *event, gpointer data)
{
    int x, y;
    GdkModifierType state;
    gdk_window_get_pointer(widget->window,&x,&y,&state);
    if(x>630&&x<958&&y>346&&y<367)
    {
        //put something to add user and pass to database
    }
}

int Register_menu()
{
    gdk_threads_enter();//this is important, before you call any gtk_* or g_* or gdk_* functions, call this function first

    Register_pixbuf=load_pixbuf_from_file(Register_menu_path); //loads the background image from files
    Register_pixbuf=gdk_pixbuf_scale_simple(Register_pixbuf,WINDOW_WIDTH,WINDOW_HEIGHT,GDK_INTERP_BILINEAR);//sets bg image to size of window
    tableU=gtk_table_new (10, 10, TRUE); //crates table for username text entry
    tableP=gtk_table_new (10, 10, TRUE); //crates table for password text entry
    tableVP=gtk_table_new (10, 10, TRUE); //crates table for verify password text entry
    image = gtk_image_new_from_pixbuf(Register_pixbuf); //sets variable for bg image
    username = gtk_entry_new(); // creates a text entry for username
    password= gtk_entry_new(); // creates a text entry for password
    verifyPassword= gtk_entry_new();  // creates a text entry for verify password
    gtk_table_attach (GTK_TABLE (tableU), username, 0, 2, 0, 4,
        GTK_EXPAND, GTK_SHRINK, 380, 168);  //add text entry to table
    gtk_table_attach (GTK_TABLE (tableP), password, 0, 2, 0, 4,
        GTK_EXPAND, GTK_SHRINK, 380, 219);  //add text entry to table
    gtk_table_attach (GTK_TABLE (tableVP), verifyPassword, 0, 2, 0, 4,
        GTK_EXPAND, GTK_SHRINK, 380, 270);  //add text entry to table
    gtk_layout_put(GTK_LAYOUT(layout), image, 0, 0);  //add bg image to layout
    gtk_container_add (GTK_CONTAINER (layout), tableU);  //add table to layout
    gtk_container_add (GTK_CONTAINER (layout), tableP);  //add table to layout
    gtk_container_add (GTK_CONTAINER (layout), tableVP);  //add table to layout
    gtk_entry_set_visibility(GTK_ENTRY(password), FALSE);  //makes text entered not be readable
    gtk_entry_set_invisible_char (GTK_ENTRY (password), '*');  //replaces letters with *
    gtk_entry_set_visibility(GTK_ENTRY(verifyPassword), FALSE);  //makes text entered not be readable
    gtk_entry_set_invisible_char (GTK_ENTRY (verifyPassword), '*');  //replaces letters with *

    
    gulong handlerID=g_signal_connect(window, "button_press_event", G_CALLBACK(Register_menu_callback),NULL); //connect signals from clicking the window to active the callback
    gtk_widget_show_all(window); //shows the window to the user
    gdk_threads_leave();//after you finich calling gtk functions, call this
    while(LoginFlag==NOT_YET_REGISTER)sleep(1);//must call sleep to release some cpu resources for gtk thread to run
    gdk_threads_enter();//again, you know what I am gonna say
    g_signal_handler_disconnect(window,handlerID); //disconnects the signals from clicking
    gdk_threads_leave();
    return LoginFlag;
}

void LoginOrRegister()
{
    while(LoginFlag==NOT_YET_LOGIN){
        Login_menu();
    }
    while(LoginFlag==NOT_YET_REGISTER){
        Register_menu();
    }
}

gint Chats_menu_callback (GtkWidget *widget, GdkEvent  *event, gpointer data)
{
    int x, y;
    GdkModifierType state;
    gdk_window_get_pointer(widget->window,&x,&y,&state);
}

/* This function rotates the position of the tabs */
void rotate_book (GtkButton *button, GtkNotebook *notebook)
{
    gtk_notebook_set_tab_pos (notebook, (notebook->tab_pos +1) %4);
}

/* Add/Remove the page tabs and the borders */
void tabsborder_book (GtkButton *button, GtkNotebook *notebook)
{
    gint tval = FALSE;
    gint bval = FALSE;
    if (notebook->show_tabs == 0)
            tval = TRUE; 
    if (notebook->show_border == 0)
            bval = TRUE;
    
    gtk_notebook_set_show_tabs (notebook, tval);
    gtk_notebook_set_show_border (notebook, bval);
}

/* Remove a page from the notebook */
void remove_book (GtkNotebook *notebook)
{
    gint page;
    
    page = gtk_notebook_current_page(notebook);
    gtk_notebook_remove_page (notebook, page);
    /* Need to refresh the widget -- 
     This forces the widget to redraw itself. */
    gtk_widget_draw(GTK_WIDGET(notebook), NULL);
}
void Chats_menu()
{
    gdk_threads_enter();//this is important, before you call any gtk_* or g_* or gdk_* functions, call this function first

    Chats_pixbuf=load_pixbuf_from_file(Chats_menu_path);  //loads the background image from files
    Chats_pixbuf=gdk_pixbuf_scale_simple(Chats_pixbuf,WINDOW_WIDTH,WINDOW_HEIGHT,GDK_INTERP_BILINEAR);  //sets bg image to size of window
    image = gtk_image_new_from_pixbuf(Chats_pixbuf);  //sets variable for bg image
    
    friendsList = gtk_list_new (); //creates list box
    gtk_list_set_selection_mode (GTK_LIST (friendsList), GTK_SELECTION_BROWSE); //sets style of list box

    //note book
    GtkWidget *windscroll;
    GtkWidget *notebook;
    GtkWidget *frame;
    GtkWidget *button;
    GtkWidget *checkbutton;
    GtkWidget *label;
    GtkWidget *notetable;
    char bufferf[32];
    char bufferl[32];
    gint    xMax = 725;
    gint    yMax = 400;
    windscroll = gtk_scrolled_window_new (NULL,NULL);
    gtk_container_border_width (GTK_CONTAINER (windscroll), 10);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (windscroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
    gtk_widget_set_usize(GTK_SCROLLED_WINDOW (windscroll), xMax, yMax);

    notetable = gtk_table_new(2,6,TRUE);
    gtk_scrolled_window_add_with_viewport (GTK_CONTAINER (windscroll), notetable);
    gtk_layout_put(GTK_LAYOUT(layout), windscroll, 20, 40);

    notebook = gtk_notebook_new ();
    gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook), GTK_POS_TOP);
    gtk_table_attach_defaults(GTK_TABLE(notetable), notebook, 0,6,0,1);
    gtk_widget_show(notebook);
    

    for (int i=0; i < 5; i++) {
        sprintf(bufferf, "Append Frame %d", i+1);
        sprintf(bufferl, "Page %d", i+1);
        
        frame = gtk_frame_new (bufferf);
        gtk_container_border_width (GTK_CONTAINER (frame), 10);
        gtk_widget_set_usize (frame, 100, 75);
        gtk_widget_show (frame);
        
        label = gtk_label_new (bufferf);
        gtk_container_add (GTK_CONTAINER (frame), label);
        gtk_widget_show (label);
        
        label = gtk_label_new (bufferl);
        gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);
    }

    checkbutton = gtk_check_button_new_with_label ("Check me please!");
    gtk_widget_set_usize(checkbutton, 100, 75);
    gtk_widget_show (checkbutton); 
    gtk_notebook_insert_page (GTK_NOTEBOOK (notebook), checkbutton, label, 2);

    for (int i=0; i < 5; i++) {
        sprintf(bufferf, "Prepend Frame %d", i+1);
        sprintf(bufferl, "PPage %d", i+1);
        
        frame = gtk_frame_new (bufferf);
        gtk_container_border_width (GTK_CONTAINER (frame), 10);
        gtk_widget_set_usize (frame, 100, 75);
        gtk_widget_show (frame);
        
        label = gtk_label_new (bufferf);
        gtk_container_add (GTK_CONTAINER (frame), label);
        gtk_widget_show (label);
        
        label = gtk_label_new (bufferl);
        gtk_notebook_prepend_page (GTK_NOTEBOOK(notebook), frame, label);
    }

    button = gtk_button_new_with_label ("remove page");
    gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
                               (GtkSignalFunc) remove_book,
                              notebook);
    gtk_table_attach_defaults(GTK_TABLE(notetable), button, 1,2,1,2);

    //gtk_container_add (GTK_CONTAINER (layout), notetable);
    //end notebook

    //testin adding items to list
    GtkWidget     *item;
    GtkWidget     *scroll;
    gint    xmax = 200;
    gint    ymax = 400;
    scroll = gtk_scrolled_window_new (NULL,NULL);
    gtk_container_border_width (GTK_CONTAINER (scroll), 10);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
    gtk_widget_set_usize(GTK_SCROLLED_WINDOW (scroll), xmax, ymax);
    gtk_scrolled_window_add_with_viewport (GTK_CONTAINER (scroll), friendsList);
    gtk_layout_put(GTK_LAYOUT(layout), scroll, 750, 40);

    
    item = gtk_list_item_new_with_label ("Armando Rodriguez");
    gtk_container_add (GTK_CONTAINER (friendsList), item);
    item = gtk_list_item_new_with_label ("Kennan Lau");
    gtk_container_add (GTK_CONTAINER (friendsList), item);
    item = gtk_list_item_new_with_label ("Michael Zeng");
    gtk_container_add (GTK_CONTAINER (friendsList), item);
    item = gtk_list_item_new_with_label ("ARIA");
    gtk_container_add (GTK_CONTAINER (friendsList), item);
    item = gtk_list_item_new_with_label ("Marchall Payatt");
    gtk_container_add (GTK_CONTAINER (friendsList), item);

    
    //gtk_layout_put(GTK_LAYOUT(layout), friendsList, 800, 40);  //add to layout
    //end test !this works!

    gtk_layout_put(GTK_LAYOUT(layout), image, 0, 0);  //add bg image to layout
    gulong handlerID=g_signal_connect(window, "button_press_event", G_CALLBACK(Chats_menu_callback),NULL);  //connect signals from clicking the window to active the callback
    gtk_widget_show_all(window);   //shows the window to the user
    gdk_threads_leave();//after you finich calling gtk functions, call this
    while(true){
        sleep(1);
    }
    
    gdk_threads_enter();//again, you know what I am gonna say
    g_signal_handler_disconnect(window,handlerID);
    gdk_threads_leave();
}
