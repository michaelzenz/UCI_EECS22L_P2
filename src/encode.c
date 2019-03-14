#include"codec.h"

//Convert the PackUnamePasswd into a json string
void encodePackUnamePasswd(char *jsonStr, PackUnamePasswd *pack)
{
   char str_action[8];
   char str_UserName[MAX_USERNAME_LEN+10];//which means that the length of user name must not exceed 30
   char str_Password[MAX_PASSWD_LEN+10];//which means that the length of password must not exceed 30
   char str_port[9];

   memset(jsonStr,'\0',sizeof(jsonStr));
   jsonStr[0]='{';
   sprintf(str_action,"\"act\":%d",pack->action);
   sprintf(str_UserName,"\"uname\":\"%s\"",pack->UserName);
   sprintf(str_Password,"\"passwd\":\"%s\"",pack->Password);
   sprintf(str_port,"\"port\":%d",pack->port);

   strcat(jsonStr,str_action);
   strcat(jsonStr,",");
   strcat(jsonStr,str_UserName);
   strcat(jsonStr,",");
   strcat(jsonStr,str_Password);
   strcat(jsonStr,",");
   strcat(jsonStr,str_port);
   strcat(jsonStr,"}");
   //Now, we have the json string {"uname":"$(UserName)","passwd":"$(PassWord)"}
   //where $(UserName) means the UserName of the user (a string)
   //and $(PassWord) means the Password of the user (a string)
   //stored in jsonStr
}

//Convert the PackAnswerLR into a json string
void encodePackAnswerLR(char *jsonStr, PackAnswerLR *pack)
{
   char str_successflag[10];
   char str_FriendList[MAX_FRIEND_NB*MAX_USERNAME_LEN+10]="\"flist\":[";
   char str_QueryPort[12];

   memset(jsonStr,'\0',sizeof(jsonStr));
   jsonStr[0]='{';

   sprintf(str_successflag,"\"flag\":%d",pack->successflag);
   int friendNb=vectorStr_count(&pack->FriendList);
   char friendName[MAX_USERNAME_LEN];
   char temp[MAX_USERNAME_LEN];
   for(int i=0;i<friendNb;i++){
      vectorStr_get(&pack->FriendList,i,temp);
      sprintf(friendName,"\"%s\"",temp);
      strcat(str_FriendList,friendName);
      if(i!=friendNb-1)strcat(str_FriendList,",");
   }
   strcat(str_FriendList,"]");
   sprintf(str_QueryPort,"\"qport\":%d", pack->QueryPort);

   strcat(jsonStr,str_successflag);
   strcat(jsonStr,",");
   strcat(jsonStr,str_FriendList);
   strcat(jsonStr,",");
   strcat(jsonStr,str_QueryPort);
   strcat(jsonStr,"}");
}

//Convert the PackAnswerLR into a json string
void encodePackQuery(char *jsonStr, PackQuery *pack)
{
   char str_UserName[MAX_USERNAME_LEN+10];
   char str_DstUserName[MAX_USERNAME_LEN+10];
   char str_MSG[MAX_MSG_LEN];
   char str_Action[10];
   char str_portNb[12];

   memset(jsonStr,'\0',sizeof(jsonStr));
   jsonStr[0]='{';

   sprintf(str_UserName,"\"uname\":\"%s\"",pack->UserName);
   sprintf(str_DstUserName,"\"dst\":\"%s\"",pack->dstUser);
   sprintf(str_MSG,"\"msg\":\"%s\"",pack->Message);
   sprintf(str_Action,"\"act\":%d",pack->action);
   sprintf(str_portNb,"\"port\":%d",pack->portNb);

   strcat(jsonStr,str_UserName);
   strcat(jsonStr,",");
   strcat(jsonStr,str_DstUserName);
   strcat(jsonStr,",");
   strcat(jsonStr,str_MSG);
   strcat(jsonStr,",");
   strcat(jsonStr,str_Action);
   strcat(jsonStr,",");
   strcat(jsonStr,str_portNb);
   strcat(jsonStr,"}");
}

void encodePackAnswerQuery(char *jsonStr, PackAnswerQuery *pack)
{
   char str_OnlineFlagList[MAX_FRIEND_NB+10]="\"ol\":\"";
   char str_Challenger[MAX_USERNAME_LEN+12];
   char str_ChallengerHost[30];
   char str_ChallengerPort[12];
   char str_messageList[MAX_MSG_LEN*MAX_FRIEND_NB+10]="\"msgs\":[";
   char str_srcUserList[MAX_USERNAME_LEN*MAX_FRIEND_NB+10]="\"srcs\":[";

   memset(jsonStr,'\0',sizeof(jsonStr));
   jsonStr[0]='{';

   memcpy(str_OnlineFlagList+strlen(str_OnlineFlagList),pack->onlineFlagList,sizeof(char)*pack->friendNumber);
   strcat(str_OnlineFlagList,"\"");

   sprintf(str_Challenger,"\"cger\":\"%s\"",pack->challenger);
   sprintf(str_ChallengerHost,"\"chost\":\"%s\"",pack->opponentHost);
   sprintf(str_ChallengerPort,"\"cport\":%d",pack->opponentPort);

   int newMsgNb=vectorStr_count(&pack->messageList);
   char temp[max(MAX_MSG_LEN,MAX_USERNAME_LEN)];
   char tempMsg[MAX_MSG_LEN];
   char tempUserName[MAX_USERNAME_LEN];
   for(int i=0;i<newMsgNb;i++){
      vectorStr_get(&pack->messageList,i,temp);
      sprintf(tempMsg,"\"%s\"",temp);
      vectorStr_get(&pack->srcUserList,i,temp);
      sprintf(tempUserName,"\"%s\"",temp);
      strcat(str_messageList,tempMsg);
      strcat(str_srcUserList,tempUserName);
      if(i!=newMsgNb-1){
         strcat(str_messageList,",");
         strcat(str_srcUserList,",");
      }

   }
   strcat(str_messageList,"]");
   strcat(str_srcUserList,"]");

   strcat(jsonStr,str_OnlineFlagList);
   strcat(jsonStr,",");
   strcat(jsonStr,str_Challenger);
   strcat(jsonStr,",");
   strcat(jsonStr,str_messageList);
   strcat(jsonStr,",");
   strcat(jsonStr,str_srcUserList);
   strcat(jsonStr,"}");
}

//Convert the PackAnswerLR into a json string
void encodePackPlay(char *jsonStr, PackPlay *pack)
{
   char str_UserName[MAX_USERNAME_LEN+10];
   char str_Action[12];
   char str_MSG[MAX_MSG_LEN+10];
   char str_start_pt[9];
   char str_end_pt[9];
   char str_promotion[9];

   char str_temp[3]="";

   memset(jsonStr,'\0',sizeof(jsonStr));
   jsonStr[0]='{';

   sprintf(str_UserName,"\"uname\":\"%s\"",pack->UserName);
   sprintf(str_Action,"\"act\":%d",pack->Action);
   sprintf(str_MSG,"\"msg\":\"%s\"",pack->message);
   sprintf(str_start_pt,"\"spt\":%d",pack->start_pt);
   sprintf(str_end_pt,"\"ept\":%d",pack->end_pt);
   sprintf(str_promotion,"\"pro\":%d",pack->promotion);

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