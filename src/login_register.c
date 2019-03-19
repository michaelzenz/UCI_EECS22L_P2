#include"server.h"

extern const char *Program;//the name of program

//this file process the login and register

Service ServiceUserPackListener={"UserPackListener", -1, false};//create a service
bool UserPackListenerShutdown=false;//as is its name describes
int UserPortSocketFD; /* socket file descriptor for service */

pthread_t UserPortLooperID;//the thread id of the login and register listener
extern int QueryPort;//as is its name describes

extern int TimeOutMicroSec;//as is its name describes


void UserServTimeOutHandler()//the hanle function for timeout
{
    if(ServiceUserPackListener.ServiceID<0)
    {
        ServiceUserPackListener.ServiceID=GetServiceID(ServiceUserPackListener.ServiceName);
        ServiceUserPackListener.isServiceRunning=true;
    }
    //PrintStatus(ServiceUserPackListener);
    printf("UserPackListener Running\n");
}

void OutputUserPack(PackUnamePasswd packUP)
{
    FILE * database;//setting up the pointer for the txt
    /*global FILE * database; /*first attept at creating file for unames*/
    if ((database = fopen("database.txt","a")) != NULL)
    {
        printf("database loaded");
    }
    else
    {
        database = fopen("database.txt","w");
    }
    //first attempt at actual package handling
    //Start decoding into a structure
    printf("beginning processing\n");

    //setting up the package to return packAnswerLR
    PackAnswerLR palr;

    //dummy friendlist info
    //will fill in once we get more values
    vectorStr friendList;
    vectorStr_init(&friendList);
    vectorStr_add(&friendList,"keenan");
    vectorStr_add(&friendList,"aria");
    palr.FriendList = friendList;


    //bases check on atoi value 
    if(packUP.action)//action 1 = registration, I dont like casting like this but oh well
    {
                printf("attempting to register %s/n",packUP.UserName);
                database = fopen("database.txt","a");
                //prints the name and password to the text file, no /n characters
                fprintf(database, "%s ",/*"name"*/packUP.UserName);
                fprintf(database, "%s ",/*"password"*/packUP.Password);
                fclose(database);
        
                palr.successflag = USER_LOGIN;//encodes seccess or registration

                //sprintf(SendBuf,"User: %s has just registered",packUP.UserName);
    }
    else//action 0 means login
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
                palr.successflag = USER_LOGIN;//encodes seccess in login
                //sprintf(SendBuf,"User: %s has just logged in",packUP.UserName);
                }
            else {
                //strcpy(SendBuf,"Invalid Password");
                palr.successflag = INVALID_PASSWD;//encodes failed password
                printf("Invalid Password\n\n");
            }
        }
        else{
        //strcpy(SendBuf,"Unknown User");
        palr.successflag = NO_SUCH_USER;//encodes failed user
        printf("Unknown User\n\n");
        }
    }
}

//as is its name describes
//packUP: the pack that contains username and password
//returns an answer login/register pack
PackAnswerLR handleLoginRegister(PackUnamePasswd packUP, char *host)
{
    //setting up the package to return packAnswerLR
    vectorStr dummy;
    vectorStr_init(&dummy);
    PackAnswerLR palr={NO_SUCH_USER,dummy};
    if(packUP.action==LOGIN){
        if(!database_isUserExist(packUP.UserName))palr.successflag=NO_SUCH_USER;
        else if(!strcmp(packUP.Password,database_get_password(packUP.UserName))){
            database_set_onlineStatus(packUP.UserName,true);
            database_set_host(packUP.UserName,host);
            palr.successflag=USER_LOGIN;
            vectorStr friends=database_get_friends(packUP.UserName);
            palr.FriendList=friends;
            palr.QueryPort=QueryPort;
        }
        else palr.successflag=INVALID_PASSWD;
    }
    else if(packUP.action==REGISTER){
        if(database_isUserExist(packUP.UserName)){
            palr.successflag=USER_ALREADY_EXIST;
        }
        else{
            database_add_user(packUP.UserName,packUP.Password,packUP.port,true);
            database_set_host(packUP.UserName,host);
            palr.successflag=USER_REGISTER;
            vectorStr dummyFriends;
            vectorStr_init(&dummyFriends);
            palr.FriendList=dummyFriends;
            palr.QueryPort=QueryPort;
            
        }
        
    }
    //OutputUserPack(packUP);
    return palr;
}

//the listener of UserPack, calls handleLoginRegister
void UserPackListener(		/* process a time request by a client */
	int DataSocketFD, char *host)
{
    int  l;

    char SendBuf[BUFFERSIZE];	/* message buffer for sending a response */
    
    char *fullbuf=readFullBuffer(DataSocketFD);
    PackUnamePasswd packUP=decodeStrPUP(fullbuf);
    

    if(packUP.action==LOGIN)printf("%s\n",fullbuf);
    free(fullbuf);
    PackAnswerLR palr=handleLoginRegister(packUP, host);

    //encoding the the packanswerLR to sendbug
    encodePackAnswerLR(SendBuf, &palr);

    l = strlen(SendBuf);
#ifdef PRINT_LOG
    printf("%s: Sending response: %s.\n", Program, SendBuf);
#endif

    int LastSendLen;
    //send back to client
    LastSendLen = write(DataSocketFD, SendBuf, l);
    if (LastSendLen < 0)FatalError("writing to data socket failed");
} /* end of ProcessRequest */

//the main looper of login/register listener
void UserPortLooper()
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
    FD_SET(UserPortSocketFD, &ActiveFDs);	/* server socket is active */
    while(!UserPackListenerShutdown)
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
        UserServTimeOutHandler();
        
	}
	else		/* some FDs have data ready to read */
	{   for(i=0; i<FD_SETSIZE; i++)
	    {   if (FD_ISSET(i, &ReadFDs))
		{   if (i == UserPortSocketFD)
		    {	/* connection request on server socket */
#ifdef DEBUG
			printf("\n%s: Accepting new client...\n", Program);
#endif
			ClientLen = sizeof(ClientAddress);
			DataSocketFD = accept(UserPortSocketFD,
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
			UserPackListener(i,inet_ntoa(ClientAddress.sin_addr));
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

//inits the user pack listener
void InitUserPackListener(int PortNo)
{
    if(PortNo<11000||PortNo>=12000)FatalError("Invalid Port Number, must be between 11000 and 11999");
    

    #ifdef PRINT_LOG
    printf("%s: Creating the User Pack listen socket...\n", Program);
    #endif
    UserPortSocketFD = MakeServerSocket(PortNo);//Make a socket
    if(UserPortSocketFD<0){
        FatalError("Fail to start user pack listener");
    }
    #ifdef PRINT_LOG
    printf("%s: Providing UserPackListener service at port %d...\n", Program, PortNo);
    #endif

    int TimeOutMicroSec=250000;

    int ret=pthread_create(&UserPortLooperID,NULL,(void*)UserPortLooper,NULL);

    if(ret!=0){
        printf("Create Local Sever thread fail, exiting\n");
        exit(1);
    }
}

//as is its name describes
void ShutUserPackListener()
{
    #ifdef PRINT_LOG
    printf("\n%s: Shutting down UserPortLooper service\n", Program);
    #endif
    UserPackListenerShutdown=true;
    pthread_join(UserPortLooperID,NULL);
    close(UserPortSocketFD);
}