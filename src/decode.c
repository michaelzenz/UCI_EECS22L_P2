#include"codec.h"


//Convert a json string into PackUnamePasswd
PackUnamePasswd decodeStrPUP(char *jsonStr)
{
    jsmn_parser json_str_parser;//create a json string parser
    jsmn_init(&json_str_parser);//init the jsmn json string parser
    PackUnamePasswd pack;
    memset(pack.UserName,'\0',sizeof(pack.UserName));
    memset(pack.Password,'\0',sizeof(pack.Password));

    jsmntok_t t[MAX_JSMN_TOKEN_NB];//the tokens for the json string
    int r=jsmn_parse(&json_str_parser, jsonStr, strlen(jsonStr),t,sizeof(t)/sizeof(t[0]));

    char temp[MAX_JSON_OBJ_LEN];//a temporary string
    memset(temp,'\0',sizeof(temp));//initialize it

    //The json string should be in the format:
    //{"uname":"$(UserName)","passwd":"$(PassWord)"}
    //where $(UserName) means the UserName of the user (a string)
    //and $(PassWord) means the Password of the user (a string)

    for(int i=1;i<r;i++){
        if(jsoneq(jsonStr,&t[i],"act")==0){
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start,jsonStr + t[i+1].start);
            pack.action=atoi(temp);
            i++;
        }
        else if(jsoneq(jsonStr,&t[i],"uname")==0){
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start, jsonStr + t[i+1].start);
            strcpy(pack.UserName,temp);
            i++;
        }
        else if(jsoneq(jsonStr,&t[i],"passwd")==0){
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start, jsonStr + t[i+1].start);
            strcpy(pack.Password,temp);
            i++;
        }
        else if(jsoneq(jsonStr,&t[i],"port")==0){
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start,jsonStr + t[i+1].start);
            pack.port=atoi(temp);
            i++;
        }
    }
    return pack;
}

//Convert a json string into PackAnswerLR
PackAnswerLR decodeStrPALR(char *jsonStr)
{
    jsmn_parser json_str_parser;//create a json string parser
    jsmn_init(&json_str_parser);//init the jsmn json string parser
    PackAnswerLR pack;
    vectorStr_init(&pack.FriendList);

    jsmntok_t t[MAX_JSMN_TOKEN_NB];//the tokens for the json string
    int r=jsmn_parse(&json_str_parser, jsonStr, strlen(jsonStr),t,sizeof(t)/sizeof(t[0]));

    char temp[MAX_JSON_OBJ_LEN];//a temporary string
    memset(temp,'\0',sizeof(temp));//initialize it

    for(int i=1;i<r;i++){
        if(jsoneq(jsonStr,&t[i],"flag")==0){
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start,jsonStr + t[i+1].start);
            pack.successflag=atoi(temp);
            i++;
        }
        else if(jsoneq(jsonStr,&t[i],"flist")==0){
			if (t[i+1].type != JSMN_ARRAY)continue; /* groups should be an array of strings */
			for (int j = 0; j < t[i+1].size; j++) {
				jsmntok_t *g = &t[i+j+2];
                sprintf(temp,"%.*s", g->end - g->start, jsonStr + g->start);
			    vectorStr_add(&pack.FriendList,temp);
            }
			i += t[i+1].size + 1;
        }
        else if(jsoneq(jsonStr,&t[i],"qport")==0){
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start,jsonStr + t[i+1].start);
            pack.QueryPort=atoi(temp);
            i++;
        }
    }
    return pack;
}

PackQuery decodeStrPQ(char *jsonStr)
{
    jsmn_parser json_str_parser;//create a json string parser
    jsmn_init(&json_str_parser);//init the jsmn json string parser
    PackQuery pack;

    jsmntok_t t[MAX_JSMN_TOKEN_NB];//the tokens for the json string
    int r=jsmn_parse(&json_str_parser, jsonStr, strlen(jsonStr),t,sizeof(t)/sizeof(t[0]));

    char temp[MAX_JSON_OBJ_LEN];//a temporary string
    memset(temp,'\0',sizeof(temp));//initialize it

    for(int i=1;i<r;i++){
        if(jsoneq(jsonStr,&t[i],"uname")==0){
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start,jsonStr + t[i+1].start);
            strcpy(pack.UserName,temp);
            i++;
        }
        else if(jsoneq(jsonStr,&t[i],"dst")==0){
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start, jsonStr + t[i+1].start);
            strcpy(pack.dstUser,temp);
            i++;
        }
        else if(jsoneq(jsonStr,&t[i],"msg")==0){
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start, jsonStr + t[i+1].start);
            strcpy(pack.Message,temp);
            i++;
        }
        else if(jsoneq(jsonStr,&t[i],"act")==0){
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start,jsonStr + t[i+1].start);
            pack.portNb=atoi(temp);
            i++;
        }
        else if(jsoneq(jsonStr,&t[i],"port")==0){
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start,jsonStr + t[i+1].start);
            pack.portNb=atoi(temp);
            i++;
        }
    }
    return pack;
}

//Convert a json string into PackAnswerLR
PackAnswerQuery decodeStrPAQ(char *jsonStr)
{
    jsmn_parser json_str_parser;//create a json string parser
    jsmn_init(&json_str_parser);//init the jsmn json string parser
    PackAnswerQuery pack;
    vectorStr_init(&pack.messageList);
    vectorStr_init(&pack.srcUserList);

    jsmntok_t t[MAX_JSMN_TOKEN_NB];//the tokens for the json string
    int r=jsmn_parse(&json_str_parser, jsonStr, strlen(jsonStr),t,sizeof(t)/sizeof(t[0]));

    char temp[MAX_JSON_OBJ_LEN];//a temporary string
    memset(temp,'\0',sizeof(temp));//initialize it

    for(int i=1;i<r;i++){
        if(jsoneq(jsonStr,&t[i],"ol")==0){
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start,jsonStr + t[i+1].start);
            memcpy(&pack.onlineFlagList,(uchar*)temp,sizeof(temp));
            i++;
        }
        else if(jsoneq(jsonStr,&t[i],"cger")==0){
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start, jsonStr + t[i+1].start);
            strcpy(pack.challenger,temp);
            i++;
        }
        else if(jsoneq(jsonStr,&t[i],"msgs")==0){
			if (t[i+1].type != JSMN_ARRAY)continue; /* groups should be an array of strings */
			for (int j = 0; j < t[i+1].size; j++) {
				jsmntok_t *g = &t[i+j+2];
                sprintf(temp,"%.*s", g->end - g->start, jsonStr + g->start);
                vectorStr_add(&pack.messageList,temp);
			}
			i += t[i+1].size + 1;
        }
        else if(jsoneq(jsonStr,&t[i],"srcs")==0){
			if (t[i+1].type != JSMN_ARRAY)continue; /* groups should be an array of strings */
			for (int j = 0; j < t[i+1].size; j++) {
				jsmntok_t *g = &t[i+j+2];
                sprintf(temp,"%.*s", g->end - g->start, jsonStr + g->start);
                vectorStr_add(&pack.srcUserList,temp);
			}
			i += t[i+1].size + 1;
        }

    }
    return pack;
}

PackPlay decodeStrPP(char *jsonStr)
{
    jsmn_parser json_str_parser;//create a json string parser
    jsmn_init(&json_str_parser);//init the jsmn json string parser
    PackPlay pack;

    jsmntok_t t[MAX_JSMN_TOKEN_NB];//the tokens for the json string
    int r=jsmn_parse(&json_str_parser, jsonStr, strlen(jsonStr),t,sizeof(t)/sizeof(t[0]));

    char temp[MAX_JSON_OBJ_LEN];//a temporary string
    memset(temp,'\0',sizeof(temp));//initialize it

    for(int i=1;i<r;i++){
        if(jsoneq(jsonStr,&t[i],"uname")==0){
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start,jsonStr + t[i+1].start);
            strcpy(pack.UserName,temp);
            i++;
        }
        else if(jsoneq(jsonStr,&t[i],"act")==0){
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start,jsonStr + t[i+1].start);
            pack.Action=atoi(temp);
            i++;
        }
        else if(jsoneq(jsonStr,&t[i],"msg")==0){
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start, jsonStr + t[i+1].start);
            strcpy(pack.message,temp);
            i++;
        }
        else if(jsoneq(jsonStr,&t[i],"spt")==0){
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start,jsonStr + t[i+1].start);
            pack.start_pt=atoi(temp);
            i++;
        }
        else if(jsoneq(jsonStr,&t[i],"ept")==0){
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start,jsonStr + t[i+1].start);
            pack.end_pt=atoi(temp);
            i++;
        }
        else if(jsoneq(jsonStr,&t[i],"pro")==0){
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start,jsonStr + t[i+1].start);
            pack.promotion=atoi(temp);
            i++;
        }
        
    }
    return pack;
}