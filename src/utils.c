#include "globals.h"
#include "utils.h"

bool exists(char *fname) {
	struct stat st = {0};

	if (stat(fname, &st) == 0) return true;
	else return false;
}
