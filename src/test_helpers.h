#ifndef TEST_HELPERS_H_
#define TEST_HELPERS_H_

#include <fcntl.h>
#include "globals.h"
#include "utils.h"

void create_dir(char *dirname);
void create_qdir();
void remove_dir(char *dirname);
void remove_qdir();
void prep_testq();

#endif
