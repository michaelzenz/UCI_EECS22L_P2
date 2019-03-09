#include"login_register.h"

extern const char *Program;//the name of program

int ServiceID=-1;
bool UserPackListenerShutdown=false;

void UserPackTimeOutHandler()//the hanle function for timeout
{
    if(ServiceID<0)ServiceID=GetServiceID("UserPackListener");
    PrintStatus(ServiceID,true);
}

PackAnswerLR handleLoginRegister(PackUnamePasswd packUP)
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
    palr.FriendNb = 2;


    //bases check on atoi value 
    if(packUP.action)//action 1 = registration, I dont like casting like this but oh well
    {
                printf("attempting to register %s/n",packUP.UserName);
                database = fopen("database.txt","a");
                //prints the name and password to the text file, no /n characters
                fprintf(database, "%s ",/*"name"*/packUP.UserName);
                fprintf(database, "%s ",/*"password"*/packUP.Password);
                fclose(database);
        
                palr.successflag = SUCCESS;//encodes seccess or registration

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
                palr.successflag = SUCCESS;//encodes seccess in login
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
    return palr;
}

void UserPackListener(		/* process a time request by a client */
	int DataSocketFD)
{
    int  l;

    char SendBuf[BUFFERSIZE];	/* message buffer for sending a response */
    
    char *fullbuf=readFullBuffer(DataSocketFD);
    PackUnamePasswd packUP=decodeStrPUP(fullbuf);
    

    if(packUP.action==LOGIN)printf("%s\n",fullbuf);
    free(fullbuf);
    PackAnswerLR palr=handleLoginRegister(packUP);

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

void UserPortLooper(		/* simple server main loop */
	int ServSocketFD,		/* server socket to wait on */
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
	    UserPackTimeOutHandler();
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
			UserPackListener(i);
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

void InitUserPackListener(int PortNo)
{
    int ServSocketFD;	/* socket file descriptor for service */
    if(PortNo<11000||PortNo>=12000)FatalError("Invalid Port Number, must be between 11000 and 11999");
    
    #ifdef PRINT_LOG
    printf("%s: Creating the User Pack listen socket...\n", Program);
    #endif
    ServSocketFD = MakeServerSocket(PortNo);//Make a socket

    #ifdef PRINT_LOG
    printf("%s: Providing UserPackListener service at port %d...\n", Program, PortNo);
    #endif

    UserPortLooper(ServSocketFD, 250000);//start the server main loop
    
    #ifdef PRINT_LOG
    printf("\n%s: Shutting down UserPackListener service\n", Program);
    #endif
    close(ServSocketFD);
}