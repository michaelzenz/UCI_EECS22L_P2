#include"connection.h"



const char *Program	/* program name for descriptive diagnostics */
	= NULL;
struct sockaddr_in
	ServerAddress;	/* server address we connect with */

int PortNo;
struct hostent *Server;

void FatalError(		/* print error diagnostics and abort */
	const char *ErrorMsg)
{
    fputs(Program, stderr);
    fputs(": ", stderr);
    perror(ErrorMsg);
    fputs(Program, stderr);
    fputs(": Exiting!\n", stderr);
    exit(20);
} /* end of FatalError */

//sets up connection and send a message to server
void sendToServer(char* msg,char*RecvBuf)
{
    int n;
    int SocketFD;

    SocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (SocketFD < 0)
    {   FatalError("socket creation failed");
    }
#ifdef PRINT_LOG
    printf("%s: Connecting to the server at port %d...\n",
		Program, ntohs(ServerAddress.sin_port));
#endif
    if (connect(SocketFD, (struct sockaddr*)&ServerAddress,
		sizeof(struct sockaddr_in)) < 0)
    {   FatalError("connecting to server failed");
    }
#ifdef PRINT_LOG
    printf("%s: Sending message '%s'...\n", Program, msg);
#endif
    n = write(SocketFD, msg, strlen(msg));
    if (n < 0)
    {   FatalError("writing to socket failed");
    }
#ifdef PRINT_LOG
    printf("%s: Waiting for response...\n", Program);
#endif
    n = read(SocketFD, RecvBuf, BUFFERSIZE-1);
    if (n < 0) 
    {   FatalError("reading from socket failed");
    }
    RecvBuf[n] = 0;
#ifdef PRINT_LOG
    printf("%s: Received response: %s\n", Program, RecvBuf);
    printf("%s: Closing the connection...\n", Program);
#endif
    close(SocketFD);
}

void ShutdownServer(		/* ask server to shutdown */
	GtkWidget *Widget,
	gpointer Data)
{
    char RecvBuf[BUFFERSIZE];
    const char *Response;

#ifdef DEBUG
    printf("%s: ShutdownServer callback starting...\n", Program);
#endif
    sendToServer("SHUTDOWN", RecvBuf);

#ifdef DEBUG
    printf("%s: ShutdownServer callback done.\n", Program);
#endif
} /* end of ShutdownServer */

//open socket and record the server address
void init_connection2server(char *program,char *host, char *port)
{
    Program=program;
#ifdef PRINT_LOG
    printf("%s: Starting...\n", Program);
#endif

    Server = gethostbyname(host);
    if (Server == NULL)
    {   fprintf(stderr, "%s: no such host named '%s'\n", Program, host);
        exit(10);
    }
    PortNo = atoi(port);
    if (PortNo <= 2000)
    {   fprintf(stderr, "%s: invalid port number %d, should be >2000\n",
		Program, PortNo);
        exit(10);
    }
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(PortNo);
    ServerAddress.sin_addr = *(struct in_addr*)Server->h_addr_list[0];


}