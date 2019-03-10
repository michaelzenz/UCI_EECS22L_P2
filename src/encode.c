#include"codec.h"

//Convert the PackUnamePasswd into a json string
void encodePackUnamePasswd(char *jsonStr, PackUnamePasswd *pack)
{
   char str_action[8]="\"act\":";
   char str_UserName[MAX_USERNAME_LEN+10]="\"uname\":\"";//which means that the length of user name must not exceed 30
   char str_Password[MAX_PASSWD_LEN+10]="\"passwd\":\"";//which means that the length of password must not exceed 30

   char str_temp[2]="";

   memset(jsonStr,'\0',sizeof(jsonStr));
   jsonStr[0]='{';
   strcat(str_action,my_itoa(pack->action,str_temp));
   strcat(str_UserName,pack->UserName);
   strcat(str_Password,pack->Password);
   strcat(jsonStr,str_action);
   strcat(jsonStr,",");
   strcat(jsonStr,str_UserName);
   strcat(jsonStr,"\",");
   strcat(jsonStr,str_Password);
   strcat(jsonStr,"\"}");
   //Now, we have the json string {"uname":"$(UserName)","passwd":"$(PassWord)"}
   //where $(UserName) means the UserName of the user (a string)
   //and $(PassWord) means the Password of the user (a string)
   //stored in jsonStr
}

//Convert the PackAnswerLR into a json string
void encodePackAnswerLR(char *jsonStr, PackAnswerLR *pack)
{
   char str_successflag[10]="\"flag\":";
   char str_FriendList[MAX_FRIEND_NB*MAX_USERNAME_LEN+10]="\"flist\":[";

   char str_temp[3]="";

   memset(jsonStr,'\0',sizeof(jsonStr));
   jsonStr[0]='{';
   strcat(str_successflag,my_itoa(pack->successflag,str_temp));

   int friendNb=vectorStr_count(&pack->FriendList);
   char friendName[MAX_USERNAME_LEN]="";
   for(int i=0;i<friendNb;i++){
      strcat(str_FriendList,"\"");
      strcat(str_FriendList,vectorStr_get(&pack->FriendList,i,friendName));
      if(i==friendNb-1){
         strcat(str_FriendList,"\"]");
         break;
      }
      strcat(str_FriendList,"\",");
   }
   if(friendNb==0)strcat(str_FriendList,"]");

   strcat(jsonStr,str_successflag);
   strcat(jsonStr,",");
   strcat(jsonStr,str_FriendList);
   strcat(jsonStr,"}");
}

//Convert the PackAnswerLR into a json string
void encodePackQuery(char *jsonStr, PackQuery *pack)
{
   char str_UserName[MAX_USERNAME_LEN+10]="\"uname\":\"";
   char str_NewFriend[MAX_USERNAME_LEN+12]="\"nfd\":\"";
   char str_MSG[MAX_MSG_LEN]="\"msg\":\"";
   char str_DstUserName[MAX_USERNAME_LEN+10]="\"dst\":\"";
   char str_portNb[12];
   char str_temp[3]="";

   memset(jsonStr,'\0',sizeof(jsonStr));
   jsonStr[0]='{';

   strcat(str_UserName,pack->UserName);
   strcat(str_UserName,"\"");
   strcat(str_NewFriend,pack->NewFriend);
   strcat(str_NewFriend,"\"");
   strcat(str_MSG,pack->Message);
   strcat(str_MSG,"\"");
   strcat(str_DstUserName,pack->dstUser);
   strcat(str_DstUserName,"\"");
   sprintf(str_portNb,"\"port\":%d",pack->portNb);

   strcat(jsonStr,str_UserName);
   strcat(jsonStr,",");
   strcat(jsonStr,str_NewFriend);
   strcat(jsonStr,",");
   strcat(jsonStr,str_MSG);
   strcat(jsonStr,",");
   strcat(jsonStr,str_DstUserName);
   strcat(jsonStr,",");
   strcat(jsonStr,str_portNb);
   strcat(jsonStr,"}");
}

void encodePackAnswerQuery(char *jsonStr, PackAnswerQuery *pack)
{
   char str_OnlineFlagList[MAX_FRIEND_NB+10]="\"ol\":\"";
   char str_Challenger[MAX_USERNAME_LEN+12]="\"cger\":\"";
   char str_messageList[MAX_MSG_LEN*MAX_FRIEND_NB+10]="\"msgs\":[";
   char str_srcUserList[MAX_USERNAME_LEN*MAX_FRIEND_NB+10]="\"srcs\":[";
   char str_QueryPort[12];


   memset(jsonStr,'\0',sizeof(jsonStr));
   jsonStr[0]='{';

   memcpy(str_OnlineFlagList+strlen(str_OnlineFlagList),pack->onlineFlagList,sizeof(char)*pack->friendNumber);
   strcat(str_OnlineFlagList,"\"");

   strcat(str_Challenger,pack->challenger);
   strcat(str_Challenger,"\"");

   int newMsgNb=vectorStr_count(&pack->messageList);
   char tempMsg[MAX_MSG_LEN]="";
   char tempUserName[MAX_USERNAME_LEN]="";
   for(int i=0;i<newMsgNb;i++){
      strcat(str_messageList,"\"");
      strcat(str_srcUserList,"\"");
      strcat(str_messageList,vectorStr_get(&pack->messageList,i,tempMsg));
      strcat(str_srcUserList,vectorStr_get(&pack->srcUserList,i,tempUserName));
      if(i==newMsgNb-1){
         strcat(str_messageList,"\"]");
         strcat(str_srcUserList,"\"]");
         break;
      }
      strcat(str_messageList,"\",");
      strcat(str_srcUserList,"\",");
   }
   sprintf(str_QueryPort,"\"qport\":%d", pack->QueryPort);




   strcat(jsonStr,str_OnlineFlagList);
   strcat(jsonStr,",");
   strcat(jsonStr,str_Challenger);
   strcat(jsonStr,",");
   strcat(jsonStr,str_messageList);
   strcat(jsonStr,",");
   strcat(jsonStr,str_srcUserList);
   strcat(jsonStr,",");
   strcat(jsonStr,str_QueryPort);
   strcat(jsonStr,"}");
}

//Convert the PackAnswerLR into a json string
void encodePackPlay(char *jsonStr, PackPlay *pack)
{
   char str_UserName[MAX_USERNAME_LEN+10]="\"uname\":\"";
   char str_Action[12]="\"act\":";
   char str_MSG[MAX_MSG_LEN+10]="\"msg\":\"";
   char str_start_pt[9]="\"spt\":";
   char str_end_pt[9]="\"ept\":";
   char str_promotion[9]="\"pro\":";

   char str_temp[3]="";

   memset(jsonStr,'\0',sizeof(jsonStr));
   jsonStr[0]='{';

   strcat(str_UserName,pack->UserName);
   strcat(str_UserName,"\"");
   strcat(str_Action,my_itoa(pack->Action,str_temp));
   strcat(str_MSG,pack->message);
   strcat(str_MSG,"\"");
   strcat(str_start_pt,my_itoa(pack->start_pt,str_temp));
   strcat(str_end_pt,my_itoa(pack->end_pt,str_temp));
   strcat(str_promotion,my_itoa(pack->promotion,str_temp));

   strcat(jsonStr,str_UserName);
   strcat(jsonStr,",");
   strcat(jsonStr,str_Action);
   strcat(jsonStr,",");
   strcat(jsonStr,str_MSG);
   strcat(jsonStr,",");
   strcat(jsonStr,str_start_pt);
   strcat(jsonStr,",");
   strcat(jsonStr,str_end_pt);
   strcat(jsonStr,",");
   strcat(jsonStr,str_promotion);
   strcat(jsonStr,"}");
}