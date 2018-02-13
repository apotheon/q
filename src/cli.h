#ifndef HELP_H_
#define HELP_H_

#include "globals.h"
#include "utils.h"

bool match_cmd(char *cmd, char *cmdtarget);
bool match_help(char *cmd);
bool match_rot(char *cmd);
bool print_help(char *self);

char * help_text();
char * usage_text(char *self);
char * try_text(char *self);

#endif
