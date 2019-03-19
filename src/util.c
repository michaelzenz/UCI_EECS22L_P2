
#include"util.h"

const char *Program=NULL;//the name of program

void FatalError(		/* print error diagnostics and abort */
	const char *ErrorMsg)
{
    fputs(Program, stderr);
    fputs(": ", stderr);
    perror(ErrorMsg);
    fputs(Program, stderr);
    fputs(": Exiting!\n", stderr);
    fflush(stderr);
    exit(20);
} /* end of FatalError */

//check if the userString matches the regex pattern
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