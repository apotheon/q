#ifndef UTILS_H_
#define UTILS_H_

#include <fcntl.h>
#include <sys/stat.h>
#include "globals.h"

bool exists(char *fname);
bool newdir(char *dir);

#endif
