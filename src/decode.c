#include"codec.h"


//Convert a json string into PackUnamePasswd
PackUnamePasswd decodeStrPUP(char *jsonStr)
{
    jsmn_parser json_str_parser;//create a json string parser
    jsmn_init(&json_str_parser);//init the jsmn json string parser
    PackUnamePasswd packUnamePasswd;
    memset(packUnamePasswd.UserName,'\0',sizeof(packUnamePasswd.UserName));
    memset(packUnamePasswd.Password,'\0',sizeof(packUnamePasswd.Password));

    jsmntok_t t[25];//the tokens for the json string
    int r=jsmn_parse(&json_str_parser, jsonStr, strlen(jsonStr),t,sizeof(t)/sizeof(t[0]));

    char temp[MAX_JSON_OBJ_LEN];//a temporary string
    memset(temp,'\0',sizeof(temp));//initialize it

    //The json string should be in the format:
    //{"uname":"$(UserName)","passwd":"$(PassWord)"}
    //where $(UserName) means the UserName of the user (a string)
    //and $(PassWord) means the Password of the user (a string)

    for(int i=1;i<r;i++)
    {
        if(jsoneq(jsonStr,&t[i],"act")==0)
        {
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start,jsonStr + t[i+1].start);
            packUnamePasswd.action=atoi(temp);
            i++;
        }
        else if(jsoneq(jsonStr,&t[i],"uname")==0)
        {
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start, jsonStr + t[i+1].start);
            strcpy(packUnamePasswd.UserName,temp);
            i++;
        }
        else if(jsoneq(jsonStr,&t[i],"passwd")==0)
        {
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start, jsonStr + t[i+1].start);
            strcpy(packUnamePasswd.Password,temp);
            i++;
        }
    }
    return packUnamePasswd;
}

//Convert a json string into PackAnswerLR
PackAnswerLR decodeStrPALR(char *jsonStr)
{
    jsmn_parser json_str_parser;//create a json string parser
    jsmn_init(&json_str_parser);//init the jsmn json string parser
    PackAnswerLR packAnswerLR;
    vectorStr_init(&packAnswerLR.FriendList);

    jsmntok_t t[50];//the tokens for the json string
    int r=jsmn_parse(&json_str_parser, jsonStr, strlen(jsonStr),t,sizeof(t)/sizeof(t[0]));

    char temp[MAX_JSON_OBJ_LEN];//a temporary string
    memset(temp,'\0',sizeof(temp));//initialize it

    //The json string should be in the format:
    //{"uname":"$(UserName)","passwd":"$(PassWord)"}
    //where $(UserName) means the UserName of the user (a string)
    //and $(PassWord) means the Password of the user (a string)

    for(int i=1;i<r;i++)
    {
        if(jsoneq(jsonStr,&t[i],"flag")==0)
        {
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start,jsonStr + t[i+1].start);
            packAnswerLR.successflag=atoi(temp);
            i++;
        }
        else if(jsoneq(jsonStr,&t[i],"fnb")==0)
        {
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start, jsonStr + t[i+1].start);
            packAnswerLR.FriendNb=atoi(temp);
            i++;
        }
        else if(jsoneq(jsonStr,&t[i],"flist")==0)
        {
			if (t[i+1].type != JSMN_ARRAY)continue; /* groups should be an array of strings */
			for (int j = 0; j < t[i+1].size; j++) {
				jsmntok_t *g = &t[i+j+2];
                sprintf(temp,"%.*s", g->end - g->start, jsonStr + g->start);
			}
			i += t[i+1].size + 1;
        }
    }
    return packAnswerLR;
}