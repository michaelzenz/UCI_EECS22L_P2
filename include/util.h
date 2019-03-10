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

typedef unsigned char uchar;

/* print error diagnostics and abort */
void FatalError(const char *ErrorMsg);

//compare regex strings, return true if is equal
bool matchRegex(const char* pattern, const char* userString);

#endif