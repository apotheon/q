#ifndef FEATURES_H_
#define FEATURES_H_

#include <fcntl.h>
#include "globals.h"
#include "cli.h"
#include "utils.h"

char *del_item(unsigned int itemno);

void add_item(char *input, char *self);
void del(char *self);
void list_all(char *self);
void print_numbered_file_listing(FILE *qfile);
void remove_item_number(unsigned int itemno);
void rot(char *self);
void rot_item(char *self);
void show_head(char *self);
void start_queuer();

#endif
