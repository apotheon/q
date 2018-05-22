#include "test_helpers.h"

bool filestringcmp(FILE *fd, char *str) {
	int nextchar = fgetc(fd);
	int n = 0;

	for (int c; (c = *(str + n)) != '\0'; ++n) {
		if (c != nextchar) return false;
		nextchar = fgetc(fd);
	}

	if (nextchar == EOF) return true;
	else return false;
}

/* execute `q list-all` to test */
bool get_line(char *line, FILE *qfile) {
	if (fgets(line, LINESIZE, qfile) > 0) return true;
	else return false;
}

bool linecmp(char *firstline, char *secondline) {
	if (strncmp(firstline, secondline, LINESIZE) == 0) return true;
	else return false;
}

bool pathcmp(char *firstpath, char *secondpath) {
	if (strncmp(firstpath, secondpath, PATH_MAX) == 0) return true;
	else return false;
}

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

void prep_testq() {
	cd(getenv("HOME"));
	create_qdir();
	cd(DIRNAME);
	if (exists(QNAME)) remove(QNAME);
	if (! exists(QNAME)) open(QNAME, O_CREAT, 0600);
	FILE *qfile = fopen(QNAME, "w");

	if (! qfile) {
		perror("Error opening file.\n");
		perror("FS may be full or unwritable.\n");
		exit(EXIT_FAILURE);
	} else {
		fprintf(qfile, "%s\n", "FIRST LINE\nSECOND LINE");
		fclose(qfile);
	}
}

void cleanup_testq() {
	cd(getenv("HOME"));

	if (exists(DIRNAME)) {
		cd(DIRNAME);
		if (exists(QNAME)) remove(QNAME);
		cd(getenv("HOME"));
		remove_qdir();
	}
}
