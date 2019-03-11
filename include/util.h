#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>
#include<stdbool.h>
#include<regex.h>

#ifndef UTIL_H
#define UTIL_H

#define TEST_MSGING

#define PRINT_LOG //if want to print log

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

typedef unsigned char uchar;

/* print error diagnostics and abort */
void FatalError(const char *ErrorMsg);

//compare regex strings, return true if is equal
bool matchRegex(const char* pattern, const char* userString);

#endif