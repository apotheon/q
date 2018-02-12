#ifndef UTILS_H_
#define UTILS_H_

#include <sys/stat.h>
#include "globals.h"

bool cd(char *dir);
bool cd_qdir();
bool check_alloc(char *p);
bool exists(char *fname);
bool get_line(char *line, FILE *qfile);
bool newdir(char *dir);
bool qexists();

void clearprint(char *text);

#endif
