#include"struct.h"
#include<string.h>
#include"jsmn.h"

#define MAX_UP_SIZE 250

//Encode Functions

void encodePackUnamePasswd(char *str_encodedPUP, PackUnamePasswd *pack);

PackUnamePasswd decodeStrUP(char *str_encodedPUP);

//Decode Functions