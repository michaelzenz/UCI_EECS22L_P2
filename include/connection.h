#include"constant.h"
#include"struct.h"
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <gtk/gtk.h>
#include <assert.h>
#include <stdlib.h>

//the buffer size to send
#define BUFFERSIZE 256

//open socket and record the server address
void init_connection2server(char *program,char *host, char *port);

//sets up connection and send a message to server
void sendToServer(char* msg,char*RecvBuf);