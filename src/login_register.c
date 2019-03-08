#include"login_register.h"

PackAnswerLR handleLoginRegister(PackUnamePasswd packUP)
{
    FILE * database;//setting up the pointer for the txt
    /*global FILE * database; /*first attept at creating file for unames*/
    if ((database = fopen("database.txt","a")) != NULL)
    {
        printf("database loaded");
        /*fprintf(database, "So begins the database %s\n", "now");
        fclose(database);*/
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