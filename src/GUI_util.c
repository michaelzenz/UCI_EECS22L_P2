#include"GUI.h"


void _ErrorMsg(char *msg)
{
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new (NULL, GTK_DIALOG_DESTROY_WITH_PARENT,
                                 GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
                                 "%s", msg);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

void guio_ErrorMsg(char *msg)
{
    gdk_threads_enter();
    _ErrorMsg(msg);
    gdk_threads_leave();
}

void _InformMsg(char *msg)
{
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new (NULL, GTK_DIALOG_DESTROY_WITH_PARENT,
                                 GTK_MESSAGE_OTHER, GTK_BUTTONS_CLOSE,
                                 "%s", msg);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

void guio_InformMsg(char *msg)
{
    gdk_threads_enter();
    _InformMsg(msg);
    gdk_threads_leave();
}

bool _AskQuestion(char *msg)
{
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new (NULL, GTK_DIALOG_DESTROY_WITH_PARENT,
                                 GTK_MESSAGE_OTHER, GTK_BUTTONS_YES_NO,
                                 "%s", msg);
    bool ret=gtk_dialog_run (GTK_DIALOG (dialog))==GTK_RESPONSE_YES;
    gtk_widget_destroy (dialog);
    return ret;
}

//Only ask yes and no
bool guio_AskQuestion(char *msg)
{
    gdk_threads_enter();
    bool ret=_AskQuestion(msg);
    gdk_threads_leave();
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