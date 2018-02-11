#ifndef UTILS_H_
#define UTILS_H_

#include <sys/stat.h>
#include "globals.h"

bool cd(char *dir);
bool cd_qdir();
bool exists(char *fname);
bool get_line(char *line, FILE *qfile);
bool newdir(char *dir);
bool qexists();

#endif
