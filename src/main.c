#include"codec.h"
#include<gtk/gtk.h>
#include<stdint.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/select.h>
#include<netdb.h>
#include<stdbool.h> 

#define BUFFERSIZE 256
#define PRINT_LOG
#define TEST1

const char *Program=NULL;
int Shutdown = 0;/* keep running until Shutdown == 1 */
char buffer[BUFFERSIZE];//the buffer to store message

char RotateLine[]={'-','\\','|','/'};
uchar RotateDirection=0;
bool FirstTimeOut=false;

#ifdef TEST1
char uname[30]="michaelz";
char passwd[30]="25619";
#endif

typedef void (*ClientHandler)(int DataSocketFD);
typedef void (*TimeoutHandler)(void);

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

void TimeOutHandleFunc(){
    if(!FirstTimeOut){
        printf("still waiting ...");
        FirstTimeOut=true;
    }
    printf("%c",RotateLine[RotateDirection]);
    fflush(stdout);
    printf("\b");
    RotateDirection=++RotateDirection%4;
}

int MakeServerSocket(		/* create a socket on this server */
	uint16_t PortNo)
{
    int ServSocketFD;
    struct sockaddr_in ServSocketName;

    /* create the socket */
    ServSocketFD = socket(PF_INET, SOCK_STREAM, 0);
    if (ServSocketFD < 0)
    {   FatalError("service socket creation failed");
    }
    /* bind the socket to this server */
    ServSocketName.sin_family = AF_INET;
    ServSocketName.sin_port = htons(PortNo);
    ServSocketName.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(ServSocketFD, (struct sockaddr*)&ServSocketName,
		sizeof(ServSocketName)) < 0)
    {   FatalError("binding the server to a socket failed");
    }
    /* start listening to this socket */
    if (listen(ServSocketFD, 5) < 0)	/* max 5 clients in backlog */
    {   FatalError("listening on socket failed");
    }
    return ServSocketFD;
} /* end of MakeServerSocket */

void ProcessRequest(		/* process a time request by a client */
	int DataSocketFD)
{
    FirstTimeOut=false;
    int  l, n;
    char RecvBuf[BUFFERSIZE];	/* message buffer for receiving a message */
    char SendBuf[BUFFERSIZE];	/* message buffer for sending a response */

    n = read(DataSocketFD, RecvBuf, sizeof(RecvBuf)-1);
    if (n < 0) 
    {   FatalError("reading from data socket failed");
    }
    RecvBuf[n] = 0;
#ifdef PRINT_LOG
    printf("%s: Received message: %s\n", Program, RecvBuf);
#endif

    #ifdef TEST1
    PackUnamePasswd packUP=decodeStrUP(RecvBuf);
    if(strcmp(packUP.UserName,"michaelz")==0){
        if(strcmp(packUP.Password,"25619")==0){
            sprintf(SendBuf,"User: %s just login",packUP.UserName);
        }
        else strcpy(SendBuf,"Invalid Password");
    }
    else{
        strcpy(SendBuf,"Unknown User");
    }
    #endif

    l = strlen(SendBuf);
#ifdef PRINT_LOG
    printf("%s: Sending response: %s.\n", Program, SendBuf);
#endif
    n = write(DataSocketFD, SendBuf, l);
    if (n < 0)FatalError("writing to data socket failed");
} /* end of ProcessRequest */

void ServerMainLoop(		/* simple server main loop */
	int ServSocketFD,		/* server socket to wait on */
	ClientHandler HandleClient,	/* client handler to call */
	TimeoutHandler HandleTimeout,	/* timeout handler to call */
	int Timeout)			/* timeout in micro seconds */
{
    int DataSocketFD;	/* socket for a new client */
    socklen_t ClientLen;
    struct sockaddr_in
	ClientAddress;	/* client address we connect with */
    fd_set ActiveFDs;	/* socket file descriptors to select from */
    fd_set ReadFDs;	/* socket file descriptors ready to read from */

    int res, i;
    struct timeval TimeVal;

    FD_ZERO(&ActiveFDs);		/* set of active sockets */
    FD_SET(ServSocketFD, &ActiveFDs);	/* server socket is active */
    while(!Shutdown)
    {
	ReadFDs = ActiveFDs;
    TimeVal.tv_sec  = Timeout / 1000000;
	TimeVal.tv_usec = Timeout % 1000000;

	/* block until input arrives on active sockets or until timeout */
	res = select(FD_SETSIZE, &ReadFDs, NULL, NULL, &TimeVal);
	if (res < 0)
	{   FatalError("wait for input or timeout (select) failed");
	}
	if (res == 0)	/* timeout occurred */
	{
	    HandleTimeout();
	}
	else		/* some FDs have data ready to read */
	{   for(i=0; i<FD_SETSIZE; i++)
	    {   if (FD_ISSET(i, &ReadFDs))
		{   if (i == ServSocketFD)
		    {	/* connection request on server socket */
#ifdef DEBUG
			printf("\n%s: Accepting new client...\n", Program);
#endif
			ClientLen = sizeof(ClientAddress);
			DataSocketFD = accept(ServSocketFD,
				(struct sockaddr*)&ClientAddress, &ClientLen);
			if (DataSocketFD < 0)
			{   FatalError("data socket creation (accept) failed");
			}
#ifdef DEBUG
			printf("%s: New client connected from %s:%hu.\n",
				Program,
				inet_ntoa(ClientAddress.sin_addr),
				ntohs(ClientAddress.sin_port));
#endif
			FD_SET(DataSocketFD, &ActiveFDs);
		    }
		    else
		    {   /* active communication with a client */
#ifdef DEBUG
			printf("%s: Dealing with client on FD%d...\n",
				Program, i);
#endif
			HandleClient(i);
#ifdef DEBUG
			printf("%s: Closing client connection FD%d.\n\n",
				Program, i);
#endif
			close(i);
			FD_CLR(i, &ActiveFDs);
		    }
		}
	    }
	}
    }
} /* end of ServerMainLoop */

int main(int argc, char *argv[]){
    int ServSocketFD;	/* socket file descriptor for service */
    int PortNo;		/* port number */
    Program=argv[0];
    #ifdef PRINT_LOG
    printf("K-Chat Server running\n");
    #endif
    if (argc < 2)
    {   
        fprintf(stderr, "Usage: %s port\n", Program);
	    exit(10);
    }
    PortNo = atoi(argv[1]);	/* get the port number */
        if (PortNo <= 2000)
    {   fprintf(stderr, "%s: invalid port number %d, should be >2000\n",
		argv[0], PortNo);
        exit(10);
    }
    #ifdef PRINT_LOG
    printf("%s: Creating the server socket...\n", Program);
    #endif
    ServSocketFD = MakeServerSocket(PortNo);

    #ifdef PRINT_LOG
    printf("%s: Creating the server ...\n", Program);

    printf("%s: Providing service at port %d...\n", Program, PortNo);
    #endif
    ServerMainLoop(ServSocketFD, ProcessRequest,
			TimeOutHandleFunc, 250000);
    #ifdef PRINT_LOG
    printf("\n%s: Shutting down.\n", Program);
    #endif
    close(ServSocketFD);
    return 0;
}