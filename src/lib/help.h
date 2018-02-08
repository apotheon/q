#ifndef HELP_H_
#define HELP_H_
#include <stdio.h>
#include <string.h>

typedef enum { false, true } bool;

bool help();
bool usage(char *self);

void try_help(char *self);

#endif
