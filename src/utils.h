#ifndef UTILS_H_
#define UTILS_H_

#ifdef __OpenBSD__
#define mem0(text, text_size) explicit_bzero
#elif __STDC_LIB_EXT1__
#define mem0(text, text_size) memset_s
#elif __linux__
/* test this on a Linux-based system some day */
/* puts("Linux memzero_explicit"); */
#define mem0(text, text_size) memzero_explicit
#else
#define mem0(text, text_size) \
for (unsigned long i = 0; i < (text_size); ++i) \
*((text) + i) = '\0'
#endif

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/stat.h>
#include "globals.h"

bool cd(char *dir);
bool cd_qdir();
bool check_alloc(char *p);
bool exists(char *fname);
bool newdir(char *dir);
bool qexists();

char *del_line(uint16_t itemno, char *fname);

void cfree(char *text, size_t text_size);
void cfreeprint(char *text, size_t text_size);
void chomp(char *text);
void clearfree(size_t text_size, int argnum, ...);
void errputs(char *text);
void set_tempname(char *tmp_file, size_t tmp_size);
void slinef(char *output_str, char *fmt_str, char *insert_str);

#endif
