#ifndef FEATURES_H_
#define FEATURES_H_

#include "globals.h"
#include "cli.h"
#include "utils.h"

char *del_item();

void add_item(char *input, char *self);
void del(char *self);
void print_numbered_file_listing(FILE *qfile);
void show_head(char *self);

#endif
