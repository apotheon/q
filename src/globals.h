#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define memzero(b,len) memset((b),0,(len))
#define DIRNAME ".quebert"
#define QNAME "queue.txt"
#define LINESIZE 1000000

typedef enum { false, true } bool;

#endif
