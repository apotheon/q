#ifndef TEST_HELPERS_H_
#define TEST_HELPERS_H_

#include <fcntl.h>
#include <limits.h>
#include "globals.h"
#include "utils.h"

bool linecmp(char *firstline, char *secondline);

void create_dir(char *dirname);
void create_qdir();
void remove_dir(char *dirname);
void remove_qdir();

void cleanup_testq();
void prep_testq();

#endif
