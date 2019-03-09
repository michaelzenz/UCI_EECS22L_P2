#include"util.h"

const char *Program=NULL;//the name of program

//the id in char array to show
char *BoardIDX[8]={"A","B","C","D","E","F","G","H"};
char *BoardIDY[8]={"1","2","3","4","5","6","7","8"};

void FatalError(		/* print error diagnostics and abort */
	const char *ErrorMsg)
{
    fputs(Program, stderr);
    fputs(": ", stderr);
    perror(ErrorMsg);
    fputs(Program, stderr);
    fputs(": Exiting!\n", stderr);
    exit(20);
} /* end of FatalError */

//output moves to a file
void print_stack_log(Node** head_ref)
{
    #ifdef PRINT_LOG
    printf("Start printing log\n");
    #endif
    FILE *fp;
    fp=fopen("bin/MovesLog.txt","w");
    char TempStr[STR_NODE_SIZE],str_SID[4],str_EID[4],str_Move[11];
    Node *TempHead=NULL;
    memset(TempStr,'\0',sizeof(TempStr));
    while(!stack_isEmpty(*head_ref)){
        stack_pop(head_ref,TempStr);
        stack_push(&TempHead,TempStr);
    }
    #ifdef PRINT_LOG
    printf("finish pushing to temp stack\n");
    #endif
    while(!stack_isEmpty(TempHead)){
        memset(str_SID,'\0',sizeof(str_SID));
        memset(str_EID,'\0',sizeof(str_EID));
        memset(str_Move,'\0',sizeof(str_Move));

        #ifdef PRINT_LOG
        printf("start poping\n");
        #endif
        stack_pop(&TempHead,TempStr);
        Move CurMove=string2move(TempStr);
        #ifdef PRINT_LOG
        printf("end decoding\n");
        #endif
        int sx=CurMove.start_pt%8,sy=CurMove.start_pt/8;
        int ex=CurMove.end_pt%8,ey=CurMove.end_pt/8;
        strcat(str_SID,BoardIDX[sx]);
        strcat(str_SID,BoardIDY[7-sy]);
        strcat(str_EID,BoardIDX[ex]);
        strcat(str_EID,BoardIDY[7-ey]);

        strcat(str_Move,str_SID);
        strcat(str_Move," to ");
        strcat(str_Move,str_EID);

        #ifdef PRINT_LOG
        printf("start outputing to log.txt\n");
        #endif
        fprintf(fp,"%s\n",str_Move);
        #ifdef PRINT_LOG
        printf("end outputing\nstart pushing back\n");
        #endif
        stack_push(head_ref,TempStr);
        #ifdef PRINT_LOG
        printf("end pushing back\n");
        #endif
    }
    fclose(fp);
    #ifdef PRINT_LOG
    printf("end printing log\n");
    #endif
}

//convert move to string
void move2string(char *str_move, Move *move)
{
    char temp[2];
    char str_piece[11]="\"piece\":";
    char str_start_pt[9]="\"spt\":";
    char str_end_pt[9]="\"ept\":";
    char str_captured[14]="\"captured\":";
    char str_cpos[10]="\"cpos\":";
    char str_smove[11]="\"smove\":";
    char str_pcstate[7]="\"pcs\":";//previous castling state
    char json_str[STR_NODE_SIZE];
    memset(json_str,'\0',sizeof(json_str));
    json_str[0]='{';
    strcat(str_piece,my_itoa(move->piece,temp));
    strcat(json_str,str_piece);
    strcat(json_str,",");
    strcat(str_start_pt,my_itoa(move->start_pt,temp));
    strcat(json_str,str_start_pt);
    strcat(json_str,",");
    strcat(str_end_pt,my_itoa(move->end_pt,temp));
    strcat(json_str,str_end_pt);
    strcat(json_str,",");
    strcat(str_captured,my_itoa(move->captured,temp));
    strcat(json_str,str_captured);
    strcat(json_str,",");
    strcat(str_cpos,my_itoa(move->captured_pos,temp));
    strcat(json_str,str_cpos);
    strcat(json_str,",");
    strcat(str_smove,my_itoa(move->special_move,temp));
    strcat(json_str,str_smove);
    strcat(json_str,",");
    strcat(str_pcstate,my_itoa(move->pre_castling_state,temp));
    strcat(json_str,str_pcstate);
    strcat(json_str,"}");

    strcpy(str_move,json_str);
}

//convert and return a move from a string
Move string2move(char *str_move)
{
    jsmn_parser str_move_parser;//the parser to parse json string
    jsmn_init(&str_move_parser);
    Move move;
    jsmntok_t t[25];
    char temp[2];
    int r=jsmn_parse(&str_move_parser, str_move,strlen(str_move),t,sizeof(t)/sizeof(t[0]));
    for(int i=1;i<r;i++)
    {
        if(jsoneq(str_move,&t[i],"piece")==0)
        {
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start,
					str_move + t[i+1].start);
            move.piece=atoi(temp);
			i++;
        }
        else if(jsoneq(str_move,&t[i],"spt")==0)
        {
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start,
					str_move + t[i+1].start);
            move.start_pt=atoi(temp);
			i++;
        }
        else if(jsoneq(str_move,&t[i],"ept")==0)
        {
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start,
					str_move + t[i+1].start);
            move.end_pt=atoi(temp);
			i++;
        }
        else if(jsoneq(str_move,&t[i],"captured")==0)
        {
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start,
					str_move + t[i+1].start);
            move.captured=atoi(temp);
			i++;
        }
        else if(jsoneq(str_move,&t[i],"cpos")==0)
        {
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start,
					str_move + t[i+1].start);
            move.captured_pos=atoi(temp);
			i++;
        }
        else if(jsoneq(str_move,&t[i],"smove")==0)
        {
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start,
					str_move + t[i+1].start);
            move.special_move=atoi(temp);
			i++;
        }
        else if(jsoneq(str_move,&t[i],"pcs")==0)
        {
            sprintf(temp, "%.*s", t[i+1].end-t[i+1].start,
					str_move + t[i+1].start);
            move.pre_castling_state=atoi(temp);
			i++;
        }
    }
    return move;
}

bool matchRegex(const char* pattern, const char* userString)
{
    bool result = false;

    regex_t regex;
    int regexInit = regcomp( &regex, pattern, REG_EXTENDED );
    if( regexInit )
    {
        //Error print : Compile regex failed
    }
    else
    {
        int reti = regexec( &regex, userString, 0, NULL, 0 );
        if( REG_NOERROR != reti )
        {
            //Error print: match failed! 
        }
        else
        {
            result = true;
        }
    }
    regfree( &regex );
    return result;
}