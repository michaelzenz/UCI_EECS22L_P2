#include"GUI.h"

extern GtkWidget *window;
extern GtkWidget *image;
extern GtkWidget *layout;//the layout to put on background and contain fixed widget
extern GtkWidget *fixed;//the widget to contain table
extern GtkWidget *table;//the widget to contain icons
extern GtkWidget *text_view; // widget to write text into log

//the pixbuf to load image and resize from a .jpg or .png file
GdkPixbuf *Login_pixbuf = NULL;
GdkPixbuf *Register_pixbuf = NULL;
GdkPixbuf *Add_friends_pixbuf = NULL;
char *Login_menu_path="res/Login.png";
char *Register_menu_path="res/Register_Menu.png";

int GameMode=0;

gint Login_menu_callback (GtkWidget *widget, GdkEvent  *event, gpointer data)
{
    int x, y;
    GdkModifierType state;
    gdk_window_get_pointer(widget->window,&x,&y,&state);
    
    printf("x:%d, y:%d\n",x,y);
    // if(x>715&&x<879&&y>238&&y<264)
    // {
    //     GameMode=GameMode_HvC;
    // }
    // else if(x>702&&x<887&&y>287&&y<319)
    // {
    //     GameMode=GameMode_HvH;
    // }
    if(x>630&&x<958&&y>346&&y<367)
    {
        Register_menu();
    }
    // else if(x>747&&x<837&&y>189&&y<214){
    //     GameMode=GameMode_ONLINE;
    // }
    // printf("GameMode:%d\n",GameMode);
}

int Login_menu()
{
    gdk_threads_enter();//this is important, before you call any gtk_* or g_* or gdk_* functions, call this function first
    Login_pixbuf=load_pixbuf_from_file(Login_menu_path);
    Login_pixbuf=gdk_pixbuf_scale_simple(Login_pixbuf,WINDOW_WIDTH,WINDOW_HEIGHT,GDK_INTERP_BILINEAR);
    
    image = gtk_image_new_from_pixbuf(Login_pixbuf);
    gtk_layout_put(GTK_LAYOUT(layout), image, 0, 0);

    
    
    gulong handlerID=g_signal_connect(window, "button_press_event", G_CALLBACK(Login_menu_callback),NULL);
    gtk_widget_show_all(window);
    gdk_threads_leave();//after you finich calling gtk functions, call this
    //while(GameMode == 4)sleep(1);//must call sleep to release some cpu resources for gtk thread to run
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
    Register_pixbuf=load_pixbuf_from_file(Register_menu_path);
    Register_pixbuf=gdk_pixbuf_scale_simple(Register_pixbuf,WINDOW_WIDTH,WINDOW_HEIGHT,GDK_INTERP_BILINEAR);
    
    image = gtk_image_new_from_pixbuf(Register_pixbuf);
    gtk_layout_put(GTK_LAYOUT(layout), image, 0, 0);

    
    gulong handlerID=g_signal_connect(window, "button_press_event", G_CALLBACK(Register_menu_callback),NULL);
    gtk_widget_show_all(window);
    gdk_threads_leave();//after you finich calling gtk functions, call this
    //while(GameMode==4)sleep(1);//must call sleep to release some cpu resources for gtk thread to run
    gdk_threads_enter();//again, you know what I am gonna say
    g_signal_handler_disconnect(window,handlerID);
    gdk_threads_leave();
    return GameMode;
}
