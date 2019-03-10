
#include"server.h"


#define STATUS_LINE_SIZE (MAX_SERVICE_NAME_LEN+12)

extern const char *Program;//the name of program

int Shutdown = 0;/* keep running until Shutdown == 1 */
char buffer[BUFFERSIZE];//the buffer to store message

vectorStr services;
vector RotateLineDirections;
char StatusLine[STATUS_LINE_SIZE];
char RotateLine[]={'-','\\','|','/'};//for showing a rotating line after still waiting...
uchar RotateDirection=0;//the current rotate direction
bool FirstTimeOut=false;//if the timeout handle function is called for the first time after the last processing request

int TimeOutMicroSec=250000;//Modify this if you want

typedef void (*ClientHandler)(int DataSocketFD);
typedef void (*TimeoutHandler)(void);

pthread_mutex_t ShowServiceStatusMutex;

void InitServiceStatusViewer()
{
    vectorStr_init(&services);
    vector_init(&RotateLineDirections);

    pthread_mutex_init(&ShowServiceStatusMutex, NULL);
}

int GetServiceID(char *service)
{
    pthread_mutex_lock(&ShowServiceStatusMutex);
    vectorStr_add(&services,service);
    vector_add(&RotateLineDirections,0);
    printf("Waiting for service:%s to start...\n",service);
    int ServiceID=vectorStr_count(&services)-1;
    pthread_mutex_unlock(&ShowServiceStatusMutex);
    return ServiceID;
}

void PrintStatus(int ServiceID, bool ServiceIsRunning)
{
    pthread_mutex_lock(&ShowServiceStatusMutex);
    int RewindBackLines=vectorStr_count(&services)-ServiceID-1;
    if(RewindBackLines!=0)RewindBackLines++;
    printf("\033[%dA",RewindBackLines);
    printf("\33[2K\r");
    fflush(stdout);
    vectorStr_get(&services,ServiceID,StatusLine);
    if(ServiceIsRunning){
        sprintf(StatusLine,"%s is running ...%c", StatusLine, 
            RotateLine[vector_get(&RotateLineDirections,ServiceID)]);
        vector_set(&RotateLineDirections,ServiceID,
            (vector_get(&RotateLineDirections,ServiceID)+1)%4);
    }
    else sprintf(StatusLine,"%s stops",StatusLine);
    printf("%s",StatusLine);
    fflush(stdout);
    printf("\033[%dB",RewindBackLines);
    pthread_mutex_unlock(&ShowServiceStatusMutex);
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
    {   printf("binding the server to a socket at port: %d failed\n",PortNo);
        return -1;
    }
    /* start listening to this socket */
    if (listen(ServSocketFD, 5) < 0)	/* max 5 clients in backlog */
    {   FatalError("listening on socket failed");
    }
    return ServSocketFD;
} /* end of MakeServerSocket */

//Read the full buffer from the given socket
//WARNING: the char* that this function returns must be free
//Otherwise there will be memory leak
char* readFullBuffer(int DataSocketFD)
{
    int LastReadLen;
    char RecvBuf[BUFFERSIZE];	/* message buffer for receiving a message */

    int lbufferSize=BUFFERSIZE;
    char *fullBuf=malloc(sizeof(char)*lbufferSize);
    memset(fullBuf,'\0',lbufferSize);
    int curLen=0;
    
    do{
        LastReadLen = read(DataSocketFD, RecvBuf, BUFFERSIZE-1);//read 
        if (LastReadLen < 0) 
        {   
            FatalError("reading from data socket failed");
        }
        RecvBuf[LastReadLen]='\0';
        if(curLen+LastReadLen+1>lbufferSize)
        {
            fullBuf=realloc(fullBuf,lbufferSize+BUFFERSIZE);
            fullBuf[curLen]='\0';
            lbufferSize+=BUFFERSIZE;
        }
        curLen+=LastReadLen;
        strcat(fullBuf,RecvBuf);
    }while(RecvBuf[LastReadLen-1]!='}');
    
#ifdef PRINT_LOG
    printf("%s: Received message: %s\n", Program, fullBuf);
#endif
    return fullBuf;
}

