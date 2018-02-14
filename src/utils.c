#include "utils.h"

bool cd(char *dir) {
	if (chdir(dir) == 0) return true;
	else return false;
}

bool cd_qdir() {
	int success = false;
	char *home = getenv("HOME");

	if (cd(home)) {
		if (! exists(DIRNAME)) newdir(DIRNAME);
		success = cd(DIRNAME);
	}

	return success;
}

/* maybe prevent allocation somehow while executing `q list-all` to test */
bool check_alloc(char *p) {
	if (p == NULL) {
		free(p);
		perror("Failed to initialize a pointer.\n");
		exit(EXIT_FAILURE);
	} else {
		return true;
	}
}

bool exists(char *fname) {
	struct stat st = {0};

	if (stat(fname, &st) == 0) return true;
	else return false;
}

/* execute `q list-all` to test */
bool get_line(char *line, FILE *qfile) {
	if (fgets(line, LINESIZE - 1, qfile) > 0) return true;
	else return false;
}

bool newdir(char *dir) {
	if (mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0) return true;
	else return false;
}

/* execute `q list-all` to test */
bool qexists() {
	return (cd_qdir() && exists(QNAME));
}

void clearprint(char *text) {
	puts(text);
	free(text);
}
