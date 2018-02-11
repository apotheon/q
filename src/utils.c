#include "utils.h"

bool cd(char *dir) {
	if (chdir(dir) == 0) return true;
	else return false;
}

/* execute `q list-all` to test this */
bool cd_qdir() {
	int success = false;
	char *home = getenv("HOME");

	if (cd(home)) {
		if (! exists(DIRNAME)) newdir(DIRNAME);
		success = cd(DIRNAME);
	}

	return success;
}

bool exists(char *fname) {
	struct stat st = {0};

	if (stat(fname, &st) == 0) return true;
	else return false;
}

/* execute `q list-all` to test this */
bool get_line(char *line, FILE *qfile) {
	if (fgets(line, LINESIZE - 1, qfile) > 0) return true;
	else return false;
}

bool newdir(char *dir) {
	if (mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0) return true;
	else return false;
}

/* execute `q list-all` to test this */
bool qexists() {
	return (cd_qdir() && exists(QNAME));
}

/* pain is short for "pointer array initializer" */
bool pain(char *parray, size_t parray_size) {
	if (parray == NULL) {
		free(parray);
		perror("Failed to initialize an array pointer.\n");
		exit(EXIT_FAILURE);
	} else {
		memzero(parray, parray_size);
		return true;
	}
}
