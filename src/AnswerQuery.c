#include"server.h"

extern const char *Program;//the name of program

int QueryServiceID=-1;
bool QueryListenerShutdown=false;
int QueryPortSocketFD; /* socket file descriptor for service */

pthread_t QueryPortLooperID;

void QueryServTimeOutHandler()//the hanle function for timeout
{
    if(QueryServiceID<0)QueryServiceID=GetServiceID("QueryPackListener");
    //PrintStatus(QueryServiceID,true);
    printf("QueryPackListener Running\n");
}

PackAnswerQuery handleQuery(PackQuery pack)
{
    //setting up the package to return packAnswerLR
    PackAnswerQuery paq;
    //OutputUserPack(packUP);
    return paq;
}

void QueryPackListener(		/* process a time request by a client */
	int DataSocketFD)
{
    int  l;

    char SendBuf[MAX_PAQ_SIZE];	/* message buffer for sending a response */
    
    char *fullbuf=readFullBuffer(DataSocketFD);
    PackQuery pack=decodeStrPQ(fullbuf);
    
    free(fullbuf);
    PackAnswerQuery paq=handleQuery(pack);

    //encoding the the packanswerLR to sendbug
    encodePackAnswerQuery(SendBuf, &paq);

    l = strlen(SendBuf);
#ifdef PRINT_LOG
    printf("%s: Sending response: %s.\n", Program, SendBuf);
#endif

    int LastSendLen;
    //send back to client
    LastSendLen = write(DataSocketFD, SendBuf, l);
    if (LastSendLen < 0)FatalError("writing to data socket failed");
} /* end of ProcessRequest */

void QueryPortLooper(		/* simple server main loop */
	int *pTimeout)			/* timeout in micro seconds */
{
    int Timeout=*pTimeout;
    int DataSocketFD;	/* socket for a new client */
    socklen_t ClientLen;
    struct sockaddr_in
	ClientAddress;	/* client address we connect with */
    fd_set ActiveFDs;	/* socket file descriptors to select from */
    fd_set ReadFDs;	/* socket file descriptors ready to read from */

    int res, i;
    struct timeval TimeVal;

    FD_ZERO(&ActiveFDs);		/* set of active sockets */
    FD_SET(QueryPortSocketFD, &ActiveFDs);	/* server socket is active */
    while(!QueryListenerShutdown)
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
	    QueryServTimeOutHandler();
	}
	else		/* some FDs have data ready to read */
	{   for(i=0; i<FD_SETSIZE; i++)
	    {   if (FD_ISSET(i, &ReadFDs))
		{   if (i == QueryPortSocketFD)
		    {	/* connection request on server socket */
#ifdef DEBUG
			printf("\n%s: Accepting new client...\n", Program);
#endif
			ClientLen = sizeof(ClientAddress);
			DataSocketFD = accept(QueryPortSocketFD,
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
			QueryPackListener(i);
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


//inits the Query Pack Listener
//returns the port that this listener uses
int InitQueryPackListener()
{
    int PortNb=11001;
    for(;PortNb<11200;PortNb++)
    {
        QueryPortSocketFD = MakeServerSocket(PortNb);//Make a socket
        if(QueryPortSocketFD>=0)break;
    }

    printf("%s: Providing QueryPackListener service at port %d...\n", Program, PortNb);

    int TimeOutMicroSec=250000;

    int ret=pthread_create(&QueryPortLooperID,NULL,(void*)QueryPortLooper,&TimeOutMicroSec);
    
    if(ret!=0){
        printf("Create Query Pack Listener thread fail, exiting\n");
        exit(1);
    }
    return PortNb;
}

void ShutQueryPackListener()
{
    #ifdef PRINT_LOG
    printf("\n%s: Shutting down QueryPortLooper service\n", Program);
    #endif
    QueryListenerShutdown=true;
    pthread_join(QueryPortLooperID,NULL);
    close(QueryPortSocketFD);
}