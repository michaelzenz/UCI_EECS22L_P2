#include"codec.h"

//Convert the a json string into PackUnamePasswd
PackUnamePasswd decodeStrUP(char *str_encodedPUP)
{
   jsmn_parser json_str_parser;//create a json string parser
   jsmn_init(&json_str_parser);//init the jsmn json string parser
   PackUnamePasswd packUnamePasswd;
   jsmntok_t t[25];//the tokens for the json string
   int r=jsmn_parse(&json_str_parser, str_encodedPUP, strlen(str_encodedPUP),t,sizeof(t)/sizeof(t[0]));

   char temp[50];//a temporary string
   memset(temp,'\0',sizeof(temp));//initialize it

   //The json string should be in the format:
   //{"uname":"$(UserName)","passwd":"$(PassWord)"}
   //where $(UserName) means the UserName of the user (a string)
   //and $(PassWord) means the Password of the user (a string)

    for(int i=1;i<r;i++)
   {
       if(jsoneq(str_encodedPUP,&t[i],"uname")==0)
       {
           sprintf(temp, "%.*s", t[i+1].end-t[i+1].start,
                    str_encodedPUP + t[i+1].start);
           strcpy(packUnamePasswd.UserName,temp);
            i++;
       }
       if(jsoneq(str_encodedPUP,&t[i],"passwd")==0)
       {
           sprintf(temp, "%.*s", t[i+1].end-t[i+1].start,
                    str_encodedPUP + t[i+1].start);
           strcpy(packUnamePasswd.Password,temp);
            i++;
       }
   }
   return packUnamePasswd;
}
