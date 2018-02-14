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

void print_invalid_command_line(int count, char **arguments);
void print_error_empty();
void print_error_exists(char *dir, char *q);
void print_error_open();
void print_error_qfile_missing(char *self);

#endif
