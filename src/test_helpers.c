#include "test_helpers.h"

void create_dir(char *dirname) {
	if (! exists(dirname)) newdir(dirname);
}

void create_qdir() {
	create_dir(DIRNAME);
}

void remove_dir(char *dirname) {
	if (exists(dirname)) rmdir(dirname);
}

void remove_qdir() {
	remove_dir(DIRNAME);
}
