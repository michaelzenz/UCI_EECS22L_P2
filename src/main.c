
#include"server.h"
#include"util.h"
#include"login_register.h"
#include"database.h"

extern const char *Program;//the name of program


int main(int argc, char *argv[]){
    test_database();
    printf("\n/****************************/\n");
    printf("This program will use the given port number to listen to login and register request\n");
    printf("And then automatically choose a port between 11000 and 11200 to listen to client query\n");
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

    InitUserPackListener(atoi(argv[1]));
    
    return 0;
}