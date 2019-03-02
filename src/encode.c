#include"codec.h"

//Convert the PackUnamePasswd into a json string
void encodePackUnamePasswd(char *str_encodedPUP, PackUnamePasswd *pack)
{
   char str_UserName[40]="\"uname\":\"";//which means that the length of user name must not exceed 30
   char str_Password[40]="\"passwd\":\"";//which means that the length of password must not exceed 30

   memset(str_encodedPUP,'\0',sizeof(str_encodedPUP));
   str_encodedPUP[0]='{';
   strcat(str_UserName,pack->UserName);
   strcat(str_Password,pack->Password);
   strcat(str_encodedPUP,str_UserName);
   strcat(str_encodedPUP,"\",");
   strcat(str_encodedPUP,str_Password);
   strcat(str_encodedPUP,"\"}");
   //Now, we have the json string {"uname":"$(UserName)","passwd":"$(PassWord)"}
   //where $(UserName) means the UserName of the user (a string)
   //and $(PassWord) means the Password of the user (a string)
   //stored in str_encodedPUP
}
