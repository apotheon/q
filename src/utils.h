#ifndef UTILS_H_
#define UTILS_H_

#include <stdarg.h>
#include <sys/stat.h>
#include "globals.h"

bool cd(char *dir);
bool cd_qdir();
bool check_alloc(char *p);
bool exists(char *fname);
bool get_line(char *line, FILE *qfile);
bool newdir(char *dir);
bool qexists();

char *del_line(char *fname);

void cfree(char *text, size_t text_size);
void clearfree(size_t text_size, int argnum, ...);
void cfreeprint(char *text, size_t text_size);

#endif
