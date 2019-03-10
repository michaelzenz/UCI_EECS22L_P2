#include"connection.h"

extern const char *Program;
extern char *UserName;
extern int QueryPort;

struct sockaddr_in ServerAddress;	/* server address we connect with */
int ServerPort;
struct hostent *Server;

char Host[15];

struct sockaddr_in ServerAddressQport;
struct hostent *ServerQport;

pthread_t QueryTaskID;
pthread_mutex_t mutex;
bool MutexInitialized=false;

bool StopQuery=false;

extern int PlayBetweenServerPort;

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


/**********************************/
//for connection to server


/*sets up connection and send a message to server
WARNING: the RecvBuf must be NULL to store the full buffer
and the char* that this function returns must be free
otherwise there will be memory leak*/
char* sendToServer(char* msg)
{
    if(!MutexInitialized){
        pthread_mutex_init(&mutex,NULL);
        MutexInitialized=true;
    }
    pthread_mutex_lock(&mutex);

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

    // free(fullRecvBuf);
    close(SocketFD);
    pthread_mutex_unlock(&mutex);

    return fullRecvBuf;
}

/*sets up connection and send a message to server
WARNING: the RecvBuf must be NULL to store the full buffer
and the char* that this function returns must be free
otherwise there will be memory leak*/
char* sendToServerQport(char* msg)
{
    if(!MutexInitialized){
        pthread_mutex_init(&mutex,NULL);
        MutexInitialized=true;
    }
    pthread_mutex_lock(&mutex);

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
    pthread_mutex_unlock(&mutex);

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

void QueryTimeredTask(char *str_PQ)
{
    while(!StopQuery){
        char *RecvBuf;
        RecvBuf=sendToServer(str_PQ);
        PackAnswerQuery paq=decodeStrPAQ(RecvBuf);

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
    strcpy(pq.NewFriend,"");
    strcpy(pq.Message,msg);
    strcpy(pq.dstUser,dstUser);
    pq.portNb=PlayBetweenServerPort;
    char str_pq[MAX_PQ_SIZE];
    encodePackQuery(str_pq,&pq);
    char *fullBuf=sendToServerQport(str_pq);
    PackAnswerQuery paq=decodeStrPAQ(fullBuf);
    #ifdef TEST_MSGING
    int newMsgNb=vectorStr_count(&paq.messageList);
    for(int i=0;i<newMsgNb;i++)
    {
        char msg[MAX_MSG_LEN];
        char srcUser[MAX_USERNAME_LEN];
        vectorStr_get(&paq.messageList,i,msg);
        vectorStr_get(&paq.srcUserList,i,srcUser);
        printf("you got a msg from %s: %s\n",srcUser,msg);
    }
    #endif
    free(fullBuf);
}

void AddNewFriend(char *newFriend)
{

}

void InitQueryTimeredTask(char *UserName, int portNb)
{
    if(!MutexInitialized){
        pthread_mutex_init(&mutex,NULL);
        MutexInitialized=true;
    }
    PackQuery pack={"","","","",portNb};
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