#include"connection.h"

extern const char *Program;

struct sockaddr_in ServerAddress;	/* server address we connect with */

int ServerPort;
struct hostent *Server;

pthread_t QueryTaskID;
pthread_mutex_t mutex;
bool MutexInitialized=false;

bool StopQuery=false;

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