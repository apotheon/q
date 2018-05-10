#ifndef UTILS_H_
#define UTILS_H_

#include <errno.h>
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

char *del_line(uint16_t itemno, char *fname);

void cfree(char *text, size_t text_size);
void cfreeprint(char *text, size_t text_size);
void chomp(char *text);
void clearfree(size_t text_size, int argnum, ...);
void errputs(char *text);
void set_tempname(char *tmp_file);
void slinef(char *output_str, char *fmt_str, char *insert_str);

#endif
