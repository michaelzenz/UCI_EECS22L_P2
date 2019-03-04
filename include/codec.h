#include"struct.h"
#include<string.h>
#include"jsmn.h"

#define MAX_UP_SIZE 250

//this header is for encode.c and decode.c

//Encode Functions

void encodePackUnamePasswd(char *str_encodedPUP, PackUnamePasswd *pack);



//Decode Functions

PackUnamePasswd decodeStrUP(char *str_encodedPUP);