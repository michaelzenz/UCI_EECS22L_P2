#include"codec.h"
#include"queueStr.h"
#include<gtk/gtk.h>
#include<stdint.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/select.h>
#include<netdb.h>
#include<stdbool.h> 
#include<stdio.h>
#include<stdlib.h>

#define BUFFERSIZE 256
#define PRINT_LOG //if want to print log
#define TEST1 //for alpha release

FILE * database;//setting up the pointer for the txt

const char *Program=NULL;//the name of program
int Shutdown = 0;/* keep running until Shutdown == 1 */
char buffer[BUFFERSIZE];//the buffer to store message

char RotateLine[]={'-','\\','|','/'};//for showing a rotating line after still waiting...
uchar RotateDirection=0;//the current rotate direction
bool FirstTimeOut=false;//if the timeout handle function is called for the first time after the last processing request

#ifdef TEST1
char uname[30]="michaelz";//hard codeded user name
char passwd[30]="25619";//hard codeded password
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

void TimeOutHandleFunc()//the hanle function for timeout
{
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
    ServSocketFD = socket(PF_INET, SOCK_STREAM, 0);//the socket descriptor representing the endpoint
    if (ServSocketFD < 0)
    {   FatalError("service socket creation failed");
    }
    /* bind the socket to this server */
    ServSocketName.sin_family = AF_INET;
    ServSocketName.sin_port = htons(PortNo);
    ServSocketName.sin_addr.s_addr = htonl(INADDR_ANY);

    //Bind the socket
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

    int lbufferSize=BUFFERSIZE;
    char *fullBuf=malloc(sizeof(char)*lbufferSize);
    memset(fullBuf,'\0',lbufferSize);
    int curLen=0;
    
    do{
        n = read(DataSocketFD, RecvBuf, BUFFERSIZE-1);//read 
        RecvBuf[n]='\0';
        if(curLen+n+1>lbufferSize)
        {
            fullBuf=realloc(fullBuf,lbufferSize+BUFFERSIZE);
            fullBuf[curLen]='\0';
            lbufferSize+=BUFFERSIZE;
        }
        curLen+=n;
        printf("pre:\n");
        printf("%s\n",fullBuf);
        printf("%s\n",RecvBuf);
        strcat(fullBuf,RecvBuf);
        printf("post:\n");
        printf("%s\n",fullBuf);
    }while(RecvBuf[n-1]!='}');
    
    

    if (n < 0) 
    {   
        FatalError("reading from data socket failed");
    }
    RecvBuf[n] = 0;
#ifdef PRINT_LOG
    printf("%s: Received message: %s\n", Program, RecvBuf);
#endif

//first attempt at actual package handling
    //Start decoding into a structure
    printf("beginning processing\n");
    PackUnamePasswd packUP=decodeStrPUP(RecvBuf);
    //And sets the send buffer based on the content

    //bases check on atoi value 
    if(packUP.action == atoi("1"))//action 1 = registration, I dont like casting like this but oh well
    {
                printf("attempting to register %s/n",packUP.UserName);
                database = fopen("database.txt","a");
                //prints the name and password to the text file, no /n characters
                fprintf(database, "%s ",/*"name"*/packUP.UserName);
                fprintf(database, "%s ",/*"password"*/packUP.Password);
                fclose(database);
                sprintf(SendBuf,"User: %s has just registered",packUP.UserName);
    }
    else if(packUP.action == atoi("0"))//action 0 means login
    {
        //current version only checks if the username and password are present in the file and not if the two are associated
        printf("attempting to log in %s %s\n",packUP.UserName, packUP.Password);
        database = fopen("database.txt","r");

        //fills the string data with the contents of the file and prints
        char data[300] = "";
        fgets(data, 300, database);
        printf("text file reads as %s\ncomparing login information\n",data);
        fclose(database);

        //comparing if the name was anywhere in the data
        char* u;
        u = strstr(data,packUP.UserName);
        //printf("%s\n",u);

        //comparing if the password was anywhere in the data
        char* p;
        p = strstr(data,packUP.Password);
        //printf(data);

        if(u){//username
           if(p){//password
                printf("User: %s has just logged in\n\n",packUP.UserName);
                sprintf(SendBuf,"User: %s has just logged in",packUP.UserName);
                }
            else {
                strcpy(SendBuf,"Invalid Password");
                printf("Invalid Password\n\n");
            }
        }
        else{
        strcpy(SendBuf,"Unknown User");
        printf("Unknown User\n\n");
        }
    }


    #ifdef TEST12
    //Start decoding into a structure
    PackUnamePasswd packUP=decodeStrUP(RecvBuf);
    //And sets the send buffer based on the content
    if(strcmp(packUP.UserName,"michaelz")==0){
        if(strcmp(packUP.Password,"25619")==0){
            database = fopen("database.txt","a");
            fprintf(database, "%s ",/*"name"*/packUP.UserName);
            fprintf(database, "%s\n",/*"number"*/packUP.Password);
            fclose(database);
            sprintf(SendBuf,"User: %s has just logged in",packUP.UserName);
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
    //send back to client
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

   /*global FILE * database; /*first attept at creating file for unames*/
    if ((database = fopen("database.txt","a")) != NULL)
    {
        printf("database loaded");
        /*fprintf(database, "So begins the database %s\n", "now");
        fclose(database);*/
    }
    else
    {
        database = fopen("database.txt","w");
    }
    
    QueueStr q;
    queueStr_init(&q);
    queueStr_enqueue(&q,"test","michael");
    queueStr_enqueue(&q,"test2","michael");
    char str_msg[MAX_MSG_LEN];
    queueStr_printAll(&q);
    queueStr_dequeue(&q,str_msg);
    queueStr_free(&q);


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
    ServSocketFD = MakeServerSocket(PortNo);//Make a socket

    #ifdef PRINT_LOG
    printf("%s: Creating the server ...\n", Program);

    printf("%s: Providing service at port %d...\n", Program, PortNo);
    #endif
    ServerMainLoop(ServSocketFD, ProcessRequest,
			TimeOutHandleFunc, 250000);//start the server main loop
    #ifdef PRINT_LOG
    printf("\n%s: Shutting down.\n", Program);
    #endif
    close(ServSocketFD);
    return 0;
}