#include"codec.h"
#include<gtk/gtk.h>
#include"vector.h"
#include<stdint.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/select.h>
#include<netdb.h>
#include<stdbool.h> 
#include<stdio.h>
#include<stdlib.h>
#include"util.h"
#include<assert.h>
#include<pthread.h>

#ifndef CONNECTION_H
#define CONNECTION_H

//this header and it`s corresponding c file 
//contains the connection functions that 
//can be used for the connection of both server and another client

//the buffer size to send
#define BUFFERSIZE 256
#define MAX_SERVICE_NAME_LEN 20

//open socket and record the server address
void init_connection2server(char *program,char *host, char *port);

//sets up connection and send a message to server
char* sendToServer(char* msg);

//Register the given service in the Service List that a 
//Status or terminal is going to show
//service: service name
int GetServiceID(char *service);

//print the status of the given service
//ServiceID: The id that attain from GetServiceID
//ServiceIsRunning: true if the service is now running
void PrintStatus(int ServiceID, bool ServiceIsRunning);

/* create a socket on this server */
//PortNo: port number
int MakeServerSocket(uint16_t PortNo);

//WARNING: the char* that this function returns must be free
//Otherwise there will be memory leak
char* readFullBuffer(int DataSocketFD);

//Send a msg to user
void SendMsgToUser(char *dstUser, char* msg);
//as is its name describes
void ChallengeUser(char *dstUser);

//init the connection to the qport of the server
void init_connection2qport();

//inits the routine query task
//as is its name describes....
void InitQueryTimeredTask();

//as is its name describes
typedef void(*AddNewFriendCallback)(char *);
//if friend is added successfully, this callback will be called
void AddNewFriend(char *newFriend, AddNewFriendCallback callback);

//as is its name describes
//same as above
typedef void(*RemoveFriendCallback)();
void DeleteFriend(char *oldFriend, RemoveFriendCallback callback);

#endif