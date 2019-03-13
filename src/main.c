
#include"server.h"
#include"util.h"
#include"database.h"

extern const char *Program;//the name of program



int main(int argc, char *argv[]){
    
    printf("\n/****************************/\n");
    printf("This program will use the given port number to listen to login and register request\n");
    printf("And then automatically choose a port between 11001 and 11200 to listen to client query\n");
    printf("Thus please make sure there is an available port in this range\n");
    printf("/****************************/\n\n");
    Program=argv[0];

    #ifdef PRINT_LOG
    printf("K-Chat Server start running\n");
    #endif

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s Login_Port\n", Program);
	    exit(10);
    }

    database_intialize();
    #ifdef TEST_MSGING
    database_add_user("michaelz","25619",-1,false);
    database_add_user("aria","no",-1,false);
    database_add_user("keenan","1",-1,false);
    database_add_friend("michaelz","aria");
    database_add_friend("michaelz","keenan");
    database_add_friend("keenan","michaelz");
    #endif

    InitServiceStatusViewer();
    int QueryPort=InitQueryPackListener();
    InitUserPackListener(atoi(argv[1]));



    while(true){
        //do sth if necessary
        sleep(2);
    }
    
    return 0;
}