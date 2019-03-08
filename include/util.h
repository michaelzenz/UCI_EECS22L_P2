#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>
#include<stdbool.h>

#ifndef UTIL_H
#define UTIL_H

#define PRINT_LOG //if want to print log

typedef unsigned char uchar;

/* print error diagnostics and abort */
void FatalError(const char *ErrorMsg);

#endif