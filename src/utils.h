#ifndef UTILS_H_
#define UTILS_H_

#include <fcntl.h>
#include <sys/stat.h>
#include "globals.h"

bool cd(char *dir);
bool exists(char *fname);
bool newdir(char *dir);

#endif
