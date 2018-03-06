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
	cd(DIRNAME);
	if (exists(QNAME)) remove(QNAME);
	cd(getenv("HOME"));
	remove_qdir();
}