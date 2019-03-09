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
GtkTextBuffer *buffer=NULL;
GList *children, *iter;


//the pixbuf to load image and resize from a .jpg or .png file
GdkPixbuf *Login_pixbuf = NULL;
GdkPixbuf *Register_pixbuf = NULL;
GdkPixbuf *Add_friends_pixbuf = NULL;
char *Login_menu_path="res/Login.png";
char *Register_menu_path="res/Register_Menu.png";
extern int GameMode;
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
    {   char Sendstr[BUFFERSIZE];
        printf("logingin\n");
        printf("the Username is %s\n",gtk_entry_get_text(username));
        printf ("The password is %s\n",gtk_entry_get_text(password));
        PackUnamePasswd packUP;
        packUP.action=LOGIN;
        strcpy(packUP.UserName,gtk_entry_get_text(username));
        strcpy(packUP.Password,gtk_entry_get_text(password)); 
        encodePackUnamePasswd(Sendstr,&packUP);
       //write(DataSocketFD,Sendstr, l);
    }
    else if(x>613&&x<747&&y>350&&y<380)
    {   
        printf("Register");
        GameMode=5;
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
    while(GameMode==4)sleep(1);//must call sleep to release some cpu resources for gtk thread to run
    gdk_threads_enter();//again, you know what I am gonna say
    g_signal_handler_disconnect(window,handlerID);
    gdk_threads_leave();
    return GameMode;
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
    empty_container(layout);
    Register_pixbuf=load_pixbuf_from_file(Register_menu_path);
    Register_pixbuf=gdk_pixbuf_scale_simple(Register_pixbuf,WINDOW_WIDTH,WINDOW_HEIGHT,GDK_INTERP_BILINEAR);
    tableU=gtk_table_new (10, 10, TRUE);
    tableP=gtk_table_new (10, 10, TRUE);
    tableVP=gtk_table_new (10, 10, TRUE);
    image = gtk_image_new_from_pixbuf(Register_pixbuf);
    username = gtk_entry_new();
    password= gtk_entry_new();
    verifyPassword= gtk_entry_new();
    gtk_table_attach (GTK_TABLE (tableU), username, 0, 2, 0, 4,
        GTK_EXPAND, GTK_SHRINK, 380, 168);
    gtk_table_attach (GTK_TABLE (tableP), password, 0, 2, 0, 4,
        GTK_EXPAND, GTK_SHRINK, 380, 219);
    gtk_table_attach (GTK_TABLE (tableVP), verifyPassword, 0, 2, 0, 4,
        GTK_EXPAND, GTK_SHRINK, 380, 270);
    gtk_layout_put(GTK_LAYOUT(layout), image, 0, 0);
    gtk_container_add (GTK_CONTAINER (layout), tableU);
    gtk_container_add (GTK_CONTAINER (layout), tableP);
    gtk_container_add (GTK_CONTAINER (layout), tableVP);
    gtk_entry_set_visibility(GTK_ENTRY(password), FALSE);
    gtk_entry_set_invisible_char (GTK_ENTRY (password), '*');
    gtk_entry_set_visibility(GTK_ENTRY(verifyPassword), FALSE);
    gtk_entry_set_invisible_char (GTK_ENTRY (verifyPassword), '*');

    
    gulong handlerID=g_signal_connect(window, "button_press_event", G_CALLBACK(Register_menu_callback),NULL);
    gtk_widget_show_all(window);
    gdk_threads_leave();//after you finich calling gtk functions, call this
    while(GameMode==5)sleep(1);//must call sleep to release some cpu resources for gtk thread to run
    gdk_threads_enter();//again, you know what I am gonna say
    g_signal_handler_disconnect(window,handlerID);
    gdk_threads_leave();
    return GameMode;
}