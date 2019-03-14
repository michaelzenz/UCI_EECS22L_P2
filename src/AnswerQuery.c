#include"server.h"

extern const char *Program;//the name of program

Service ServiceQueryListener={"QueryListener", -1, false};
bool QueryListenerShutdown=false;
int QueryPortSocketFD; /* socket file descriptor for service */

pthread_t QueryPortLooperID;
extern int TimeOutMicroSec;//Modify this if you want

int QueryPort;

bool isQueryServStatusPrinting=false;

void QueryServTimeOutHandler()//the hanle function for timeout
{
    if(ServiceQueryListener.ServiceID<0)
    {
        ServiceQueryListener.ServiceID=GetServiceID(ServiceQueryListener.ServiceName);
        ServiceQueryListener.isServiceRunning=true;
    }
    //PrintStatus(ServiceQueryListener);
    isQueryServStatusPrinting=false;
    printf("QueryPackListener Running\n");
}

PackAnswerQuery handleQuery(PackQuery pack,char *host)
{
    //setting up the package to return packAnswerLR
    
    PackAnswerQuery paq;
    if(database_isUserExist(pack.UserName)){
        if(pack.action!=QUERY_CHECK_UPDATE)
        {

        }
        database_set_host(pack.UserName,host);
        database_set_port(pack.UserName,pack.portNb);
        if(database_isUserExist(pack.dstUser)&&strlen(pack.Message)>0){
            database_add_msg(pack.dstUser,pack.Message,pack.UserName);
        }
        if(pack.action==QUERY_ADD_FRIEND&&database_isUserExist(pack.dstUser)){
            bool ret = database_add_friend(pack.UserName,pack.dstUser);
            if (ret == false)
                paq.opponentPort = -2; //failure
            else{
                strcpy(paq.challenger,pack.dstUser);
                paq.opponentPort = -4; //success
            }
                
        }
        if(pack.action==QUERY_DELETE_FRIEND&&database_isUserExist(pack.dstUser)){
            bool ret = database_delete_friend(pack.UserName,pack.dstUser);
            if (ret == false)
                paq.opponentPort = -2; //failure
            else{
                paq.opponentPort = -4; // success
                strcpy(paq.challenger,pack.dstUser);
            }
                
        }
        if(pack.action==QUERY_CHALLENGE&&database_get_onlineStatus(pack.dstUser)){
            database_add_challenger(pack.dstUser,pack.UserName);
            strcpy(paq.challenger,pack.UserName);
            strcpy(paq.opponentHost,database_get_host(pack.dstUser));
            paq.opponentPort=database_get_port(pack.dstUser);
        }
        else{
            //for chllenger
            QNodeChallenger node=database_get_nextChallenger(pack.UserName);
            if(node.challenger!=NULL){
                strcpy(paq.challenger,node.challenger);
                strcpy(paq.opponentHost,database_get_host(node.challenger));
                paq.opponentPort=database_get_port(node.challenger);
            }
            else
            {
                strcpy(paq.challenger,"");
                strcpy(paq.opponentHost,"");
                paq.opponentPort=-1;
            }
        }
        //for online status
        vectorStr friends=database_get_friends(pack.UserName);
        int friendNumber=vectorStr_count(&friends);
        paq.friendNumber=friendNumber;
        char temp[MAX_USERNAME_LEN];
        for(int i=0;i<friendNumber;i++){
            paq.onlineFlagList[i]=database_get_onlineStatus(vectorStr_get(&friends,i,temp));
        }
        //for msg and srcUser
        QueueChat *pMSGqueue=database_get_msgQueue(pack.UserName);
        vectorStr messageList,srcUserList;
        vectorStr_init(&messageList);
        vectorStr_init(&srcUserList);
        while(!queueChat_isEmpty(pMSGqueue)){
            QNodeMsg node=queueChat_dequeueMsg(pMSGqueue);
            vectorStr_add(&messageList,node.msg);
            vectorStr_add(&srcUserList,node.srcUser);
            queueChat_freeMsgNode(&node);
        }
        paq.messageList=messageList;
        paq.srcUserList=srcUserList;
    }
    //OutputUserPack(packUP);
    return paq;
}

void QueryPackListener(		/* process a time request by a client */
	int DataSocketFD, char *host)
{
    int  l;

    char SendBuf[MAX_PAQ_SIZE];	/* message buffer for sending a response */
    
    char *fullbuf=readFullBuffer(DataSocketFD);
    PackQuery pack=decodeStrPQ(fullbuf);
    
    free(fullbuf);
    PackAnswerQuery paq=handleQuery(pack,host);

    //encoding the the packanswerLR to sendbug
    encodePackAnswerQuery(SendBuf, &paq);
    printf("Sending back:%s to query\n",SendBuf);

    l = strlen(SendBuf);
#ifdef PRINT_LOG
    printf("%s: Sending response: %s.\n", Program, SendBuf);
#endif

    int LastSendLen;
    //send back to client
    LastSendLen = write(DataSocketFD, SendBuf, l);
    if (LastSendLen < 0)FatalError("writing to data socket failed");
} /* end of ProcessRequest */

void QueryPortLooper()
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
        if(!isQueryServStatusPrinting){
            isQueryServStatusPrinting=true;
            // pthread_t new_printer;
            // pthread_create(&new_printer,NULL,(void*)QueryServTimeOutHandler,NULL);
            QueryServTimeOutHandler();
        }
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
			QueryPackListener(i,inet_ntoa(ClientAddress.sin_addr));
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
    QueryPort=PortNb;


    int ret=pthread_create(&QueryPortLooperID,NULL,(void*)QueryPortLooper,NULL);
    
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