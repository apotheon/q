#ifndef TEST_HELPERS_H_
#define TEST_HELPERS_H_

#include <fcntl.h>
#include <limits.h>
#include "globals.h"
#include "utils.h"

bool filestringcmp(FILE *fd, char *str);
bool get_line(char *line, FILE *qfile);
bool linecmp(char *firstline, char *secondline);

bool test_error_no_queue(char *tested_command, char *output);

void create_dir(char *dirname);
void create_qdir();
void remove_dir(char *dirname);
void remove_qdir();

void cleanup_testq();
void prep_testq();

#endif
