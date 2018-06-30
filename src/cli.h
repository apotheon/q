#ifndef CLI_H_
#define CLI_H_

#include <err.h>
#include "globals.h"
#include "features.h"
#include "utils.h"

bool match_cmd(char *cmd, char *cmdtarget);
bool match_help(char *cmd);
bool match_rot(char *cmd);
bool print_help(char *self);

char *help_text();
char *usage_text(char *self);
char *try_text(char *self);

void cmd_with_arg(int count, char **arguments, char *cmd);
void print_invalid_command_line(int count, char **arguments);
void print_error_qfile_exists(char *dir, char *q);
void print_error_qfile_missing(char *self);

#endif
