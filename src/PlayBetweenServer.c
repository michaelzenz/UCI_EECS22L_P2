#include"PlayBetween.h"



extern const char *Program;

int Shutdown = 0;/* keep running until Shutdown == 1 */

bool firstTimeOut=true;
bool ServicesListInit=false;
char RotateLine[]={'-','\\','|','/'};//for showing a rotating line after still waiting...
uchar RotateDirection=0;//the current rotate direction

int PlayBetweenSocketFD;
int PlayBetweenServerPort;

pthread_t PlayBetweenLooperID;
OnlinePlayCallback *RecvCallback;

int TimeOutMicroSec=250000;

extern bool isPlayingWithOpponent;

void PlayBetweenTimeOutHandler()//the hanle function for timeout
{
    if(firstTimeOut){
        printf("local server running... ");
        firstTimeOut=false;
    }
    printf("%c",RotateLine[RotateDirection]);
    fflush(stdout);
    RotateDirection=++RotateDirection%4;
    printf("\b");
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
		sizeof(ServSocketName)) < 0){
        printf("Fail to bind server to socket, port %d may has been used\n",PortNo);
        return -1;
    }
    /* start listening to this socket */
    if (listen(ServSocketFD, 5) < 0)	/* max 5 clients in backlog */
    {   FatalError("listening on socket failed");
    }
    return ServSocketFD;
} /* end of MakeServerSocket */



void PlayPackListener(int DataSocketFD)
{
    firstTimeOut=false;
    int  l;
    
    char *fullbuf=readFullBuffer(DataSocketFD);
    if(matchRegex(fullbuf,"color:*")){

    }
    else{
        PackPlay packPlay=decodeStrPP(fullbuf);
        RecvCallback->RecvCallback(packPlay);
    }

    

    free(fullbuf);

    //TODO: answer request
    char *SendBuf=PLAY_PACK_RECEIVED;

    l = strlen(SendBuf);
#ifdef PRINT_LOG
    printf("%s: Sending response: %s.\n", Program, SendBuf);
#endif

    int LastSendLen;
    //send back to client
    LastSendLen = write(DataSocketFD, SendBuf, l);
    if (LastSendLen < 0)FatalError("writing to data socket failed");
}

void PlayBetweenLooper()
{
    int Timeout=TimeOutMicroSec;
    int DataSocketFD;	/* socket for a new client */
    socklen_t ClientLen;
    struct sockaddr_in
	ClientAddress;	/* client address we connect with */
    fd_set ActiveFDs;	/* socket file descriptors to select from */
    fd_set ReadFDs;	/* socket file descriptors ready to read from */

    int res, i;
    struct timeval TimeVal;

    FD_ZERO(&ActiveFDs);		/* set of active sockets */
    FD_SET(PlayBetweenSocketFD, &ActiveFDs);	/* server socket is active */
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
	    PlayBetweenTimeOutHandler();
	}
	else		/* some FDs have data ready to read */
	{   for(i=0; i<FD_SETSIZE; i++)
	    {   if (FD_ISSET(i, &ReadFDs))
		{   if (i == PlayBetweenSocketFD)
		    {	/* connection request on server socket */
#ifdef DEBUG
			printf("\n%s: Accepting new client...\n", Program);
#endif
			ClientLen = sizeof(ClientAddress);
			DataSocketFD = accept(PlayBetweenSocketFD,
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
			PlayPackListener(i);
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

void PPcalllbackFunc(PackPlay pack)
{
    if(!isPlayingWithOpponent)return;
    if(pack.Action==PLAYBETWEEN_CHAT){
        
    }
    else if(pack.Action==PLAYBETWEEN_PLAY){
        env_play2(RecvCallback->pGameState,pack.start_pt,pack.end_pt,0);
    }
    else if(pack.Action==PLAYBETWEEN_UNDO)
        env_undo(RecvCallback->pGameState);
}



//callback: this function will be called when a PackPlay is received
int InitPlayBetweenServer(OnlinePlayCallback *callback)
{
    #ifdef PRINT_LOG
    printf("%s: Creating the User Pack listen socket...\n", Program);
    #endif
    int PortNb=11200;
    for(;PortNb<12000;PortNb++)
    {
        PlayBetweenSocketFD = MakeServerSocket(PortNb);//Make a socket
        if(PlayBetweenSocketFD>=0)break;
    }

    printf("%s: Providing PlayPackListener service at port %d...\n", Program, PortNb);
    PlayBetweenServerPort=PortNb;
    callback->RecvCallback=PPcalllbackFunc;
    RecvCallback=callback;
    
    //start the PlayBetweenLooper
    int ret=pthread_create(&PlayBetweenLooperID,NULL,(void*)PlayBetweenLooper,NULL);

    if(ret!=0){
        printf("Create Local Sever thread fail, exiting\n");
        exit(1);
    }

    return PortNb;
}

void ShutPlayBetweenServer()
{
    #ifdef PRINT_LOG
    printf("\n%s: Shutting down UserPackListener service\n", Program);
    #endif
    Shutdown=1;
    pthread_join(PlayBetweenLooperID,NULL);
    close(PlayBetweenSocketFD);
}
