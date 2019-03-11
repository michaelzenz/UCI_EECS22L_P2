#include"PlayBetween.h"

extern const char *Program;

//here oppo is just a brief of opponent

//for connection to server
struct sockaddr_in OppoServerAddress;	/* server address we connect with */

int OppoServerPort;
struct hostent *OppoServer;

//sets up connection and send a message to server
//WARNING: the RecvBuf must be NULL to store the full buffer
//and the char* that this function returns must be free
//otherwise there will be memory leak
char* sendToOppo(char* msg)
{
    int n;
    int SocketFD;

    SocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (SocketFD < 0)
    {   FatalError("socket creation failed");
    }
#ifdef PRINT_LOG
    printf("%s: Connecting to the server at port %d...\n",
		Program, ntohs(OppoServerAddress.sin_port));
#endif
    if (connect(SocketFD, (struct sockaddr*)&OppoServerAddress,
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

    char *fullRecvBuf=readFullBuffer(SocketFD);
    
#ifdef PRINT_LOG
    printf("%s: Received response: %s\n", Program, fullRecvBuf);
    printf("%s: Closing the connection...\n", Program);
#endif

    // free(fullRecvBuf);
    close(SocketFD);
    return fullRecvBuf;
}

//open socket and record the server address
void init_connection2oppo(char *host, char *port)
{
#ifdef PRINT_LOG
    printf("%s: Starting...\n", Program);
#endif

    OppoServer = gethostbyname(host);
    if (OppoServer == NULL)
    {   fprintf(stderr, "%s: no such host named '%s'\n", Program, host);
        exit(10);
    }
    OppoServerPort = atoi(port);
    if (OppoServerPort <= 2000)
    {   fprintf(stderr, "%s: invalid port number %d, should be >2000\n",
		Program, OppoServerPort);
        exit(10);
    }
    OppoServerAddress.sin_family = AF_INET;
    OppoServerAddress.sin_port = htons(OppoServerPort);
    OppoServerAddress.sin_addr = *(struct in_addr*)OppoServer->h_addr_list[0];
}