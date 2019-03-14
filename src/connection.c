#include"connection.h"
#include"PlayBetween.h"
#include"msgChat.h"
#include"GUI.h"

//everything to do with server is in this file

extern const char *Program;
char *UserName;
int QueryPort;
vectorStr FriendsList;

struct sockaddr_in ServerAddress;//for user port	/* server address we connect with */
int ServerPort;
struct hostent *Server;

char Host[15];

struct sockaddr_in ServerAddressQport;//for qport
struct hostent *ServerQport;

pthread_t QueryTaskID;
pthread_mutex_t mutex;
bool MutexInitialized=false;

bool StopQuery=false;

extern int PlayBetweenServerPort;//for client to client
extern OnlinePlayer localPlayer, remotePlayer;
bool isPlayingWithOpponent=false;
bool isWaitingChallengeRespose=false;

//WARNING: the char* that this function returns must be free
//Otherwise there will be memory leak
//WARNING2: this function only reads a full json string
//other that json string must not be longer that BUFFERSIZE
//read whatever you receive
char* readFullBuffer(int DataSocketFD)
{
    int LastReadLen;
    char RecvBuf[BUFFERSIZE];	/* message buffer for receiving a message */

    int lbufferSize=BUFFERSIZE;
    char *fullBuf=malloc(sizeof(char)*lbufferSize);
    memset(fullBuf,'\0',lbufferSize);
    int curLen=0;
    bool firstRead=true;
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
        if(firstRead){
            if(RecvBuf[0]!='{')break;
            firstRead=false;
        }
    }while(RecvBuf[LastReadLen-1]!='}');
    
#ifdef PRINT_LOG
    printf("%s: Received message: %s\n", Program, fullBuf);
#endif
    return fullBuf;
}


/**********************************/
//for connection to server


/*sets up connection and send a message to server
WARNING: the RecvBuf must be NULL to store the full buffer
and the char* that this function returns must be free
otherwise there will be memory leak*/
//WARNING: the char* that this function returns must be free
//Otherwise there will be memory leak
//send to server user port
char* sendToServer(char* msg)
{
    if(!MutexInitialized){
        pthread_mutex_init(&mutex,NULL);
        MutexInitialized=true;
    }
    //pthread_mutex_lock(&mutex);

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

    char *fullRecvBuf=readFullBuffer(SocketFD);
    
#ifdef PRINT_LOG
    printf("%s: Received response: %s\n", Program, fullRecvBuf);
    printf("%s: Closing the connection...\n", Program);
#endif

    //free(fullRecvBuf);
    close(SocketFD);
    //pthread_mutex_unlock(&mutex);

    return fullRecvBuf;
}

/*sets up connection and send a message to server
WARNING: the RecvBuf must be NULL to store the full buffer
and the char* that this function returns must be free
otherwise there will be memory leak*/
//send to server qport
char* sendToServerQport(char* msg)
{
    if(!MutexInitialized){
        pthread_mutex_init(&mutex,NULL);
        MutexInitialized=true;
    }
    //pthread_mutex_lock(&mutex);

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
    if (connect(SocketFD, (struct sockaddr*)&ServerAddressQport,
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
    //pthread_mutex_unlock(&mutex);

    return fullRecvBuf;
}

//open socket and record the server address
void init_connection2server(char *program,char *host, char *port)
{
    Program=program;
#ifdef PRINT_LOG
    printf("%s: Starting...\n", Program);
#endif
    strcpy(Host,host);
    Server = gethostbyname(host);
    if (Server == NULL)
    {   fprintf(stderr, "%s: no such host named '%s'\n", Program, host);
        exit(10);
    }
    ServerPort = atoi(port);
    if (ServerPort <= 2000)
    {   fprintf(stderr, "%s: invalid port number %d, should be >2000\n",
		Program, ServerPort);
        exit(10);
    }
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(ServerPort);
    ServerAddress.sin_addr = *(struct in_addr*)Server->h_addr_list[0];
}

//open socket and record the server address
void init_connection2qport()
{
#ifdef PRINT_LOG
    printf("%s: Starting...\n", Program);
#endif

    ServerQport = gethostbyname(Host);
    if (ServerQport == NULL)
    {   fprintf(stderr, "%s: no such host named '%s'\n", Program, Host);
        exit(10);
    }

    ServerAddressQport.sin_family = AF_INET;
    ServerAddressQport.sin_port = htons(QueryPort);
    ServerAddressQport.sin_addr = *(struct in_addr*)ServerQport->h_addr_list[0];
}
void stopWaitingForResponse(void *pdata){
    isWaitingChallengeRespose=false;
}

//
void handlePAQ(PackAnswerQuery paq)
{
    if(!isWaitingChallengeRespose&&strlen(paq.challenger)>0){
        init_connection2oppo(paq.opponentHost,paq.opponentPort);
        if(!strcmp(UserName,paq.challenger)){
            printf("got opponent server, wait for response\n");
            isWaitingChallengeRespose=true;
            //gdk_threads_enter();
            guio_waitUserActionWithCallback("Waiting for the opponent to accept challenge",
                stopWaitingForResponse, NULL);
            //gdk_threads_leave();
            
        }
        else{
            char question[70];
            sprintf(question,
                "%s wants to challenge you, do you want to accept?",paq.challenger);
            gdk_threads_enter();
            bool acceptChallenge=guio_AskQuestion(question);
            gdk_threads_leave();
            if(acceptChallenge){
                gdk_threads_enter();
                bool LocalIsWhite=guio_AskQuestion("Do you want to play as WHITE?");
                gdk_threads_leave();
                char *recvFromOppo=NULL;
                if(LocalIsWhite)recvFromOppo=sendToOppo("iChooseColor:w");
                else recvFromOppo=sendToOppo("iChooseColor:b");
                if(!strcmp(recvFromOppo,CHALLENGE_IS_CANCELED)){
                    gdk_threads_enter();
                    guio_InformMsg("Your Opponent has canceled the challenge");
                    gdk_threads_leave();
                }
                else if(!strcmp(COLOR_SELECTION_RECEIVED,recvFromOppo)){
                    strcpy(remotePlayer.UserName,paq.challenger);
                    remotePlayer.color=LocalIsWhite?BLACK:WHITE;
                    localPlayer.color=LocalIsWhite?WHITE:BLACK;
                    isPlayingWithOpponent=true;
                }
                else
                    FatalError("Your Opponent fail to receive your color selection\n");
                if(recvFromOppo!=NULL)free(recvFromOppo);
            }
        }
    }

    int NewMsgCnt=vectorStr_count(&paq.messageList);
    char msg[MAX_MSG_LEN],user[MAX_USERNAME_LEN];
    for(int i=0;i<NewMsgCnt;i++){
        printf("you got a new message:%s from %s\n",
            vectorStr_get(&paq.messageList,i,msg),
            vectorStr_get(&paq.srcUserList,i,user));
        if(!msgChat_get_isUserExist(user)){
            msgChat_addUser(user,false);
            gdk_threads_enter();
            guio_addUnkown(user);
            gdk_threads_leave();
        }
        msgChat_add_msg(user,msg);
    }
    if(NewMsgCnt>0)guio_onMsgUpdate();
}

void QueryTimeredTask(char *str_PQ)
{
    while(!StopQuery){
        char *RecvBuf;
        RecvBuf=sendToServerQport(str_PQ);
        PackAnswerQuery paq=decodeStrPAQ(RecvBuf);
        
        handlePAQ(paq);
        
        free(RecvBuf);
        sleep(2);
    }
    free(str_PQ);
}

void SendMsgToUser(char *dstUser, char* msg)
{
    #ifdef PRINT_LOG
    printf("sending msg to %s\n",dstUser);
    #endif
    printf("sending msg to %s\n",dstUser);
    PackQuery pq;
    strcpy(pq.UserName,UserName);
    strcpy(pq.dstUser,dstUser);
    strcpy(pq.Message,msg);
    pq.action=QUERY_CHAT;
    pq.portNb=PlayBetweenServerPort;
    char str_pq[MAX_PQ_SIZE];
    encodePackQuery(str_pq,&pq);
    char *fullBuf=sendToServerQport(str_pq);
    PackAnswerQuery paq=decodeStrPAQ(fullBuf);

    handlePAQ(paq);

    free(fullBuf);
}

void ChallengeUser(char *dstUser)
{
    #ifdef PRINT_LOG
    printf("sending msg to %s\n",dstUser);
    #endif
    printf("sending msg to %s\n",dstUser);
    PackQuery pq;
    strcpy(pq.UserName,UserName);
    strcpy(pq.dstUser,dstUser);
    strcpy(pq.Message,"");
    pq.action=QUERY_CHALLENGE;
    pq.portNb=PlayBetweenServerPort;
    char str_pq[MAX_PQ_SIZE];
    encodePackQuery(str_pq,&pq);
    char *fullBuf=sendToServerQport(str_pq);
    printf("%s\n",fullBuf);
    PackAnswerQuery paq=decodeStrPAQ(fullBuf);

    handlePAQ(paq);

    free(fullBuf);
}

//Work in progress
void AddNewFriend(char *newFriend)
{
    #ifdef PRINT_LOG
    printf("adding friend %s\n",dstUser);
    #endif
    printf("adding friend %s\n",newFriend);
    PackQuery pq;
    strcpy(pq.UserName,UserName);
    strcpy(pq.dstUser,newFriend);
    strcpy(pq.Message,"");
    pq.action=QUERY_ADD_FRIEND;
    pq.portNb=PlayBetweenServerPort;
    char str_pq[MAX_PQ_SIZE];
    encodePackQuery(str_pq,&pq);
    char *fullBuf=sendToServerQport(str_pq);
    printf("%s\n",fullBuf);
    PackAnswerQuery paq=decodeStrPAQ(fullBuf);

    handlePAQ(paq);

    free(fullBuf);
}
//work in progress
void DeleteFriend(char *oldFriend)
{
    #ifdef PRINT_LOG
    printf("adding friend %s\n",dstUser);
    #endif
    printf("adding friend %s\n",oldFriend);
    PackQuery pq;
    strcpy(pq.UserName,UserName);
    strcpy(pq.dstUser,oldFriend);
    strcpy(pq.Message,"");
    pq.action=QUERY_DELETE_FRIEND;
    pq.portNb=PlayBetweenServerPort;
    char str_pq[MAX_PQ_SIZE];
    encodePackQuery(str_pq,&pq);
    char *fullBuf=sendToServerQport(str_pq);
    printf("%s\n",fullBuf);
    PackAnswerQuery paq=decodeStrPAQ(fullBuf);

    handlePAQ(paq);

    free(fullBuf);
}

void InitQueryTimeredTask()
{
    printf("Start Regular Query Check\n");
    if(!MutexInitialized){
        pthread_mutex_init(&mutex,NULL);
        MutexInitialized=true;
    }
    PackQuery pack={"","","",QUERY_CHECK_UPDATE,PlayBetweenServerPort};
    strcpy(pack.UserName,UserName);
    char *str_PQ=malloc(sizeof(char)*MAX_PQ_SIZE);
    encodePackQuery(str_PQ,&pack);
    int ret=pthread_create(&QueryTaskID,NULL,(void*)QueryTimeredTask,str_PQ);

    if(ret!=0){
        printf("Create Local Sever thread fail, exiting\n");
        exit(1);
    }
}

void StopQueryTimeredTask()
{
    StopQuery=true;
    pthread_join(QueryTaskID,NULL);
    pthread_mutex_destroy(&mutex);
}