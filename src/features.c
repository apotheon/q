#include "features.h"

void add_item(char *input, char *self) {
	if (qexists()) {
		FILE *qfile = fopen(QNAME, "a");

		if (! qfile) print_error_open();
		else fprintf(qfile, "%s\n", input);

		fclose(qfile);
	} else {
		print_error_qfile_missing(self);
	}
}

char *del_item() {
	return del_line(QNAME);
}

void del(char *self) {
	if (qexists()) {
		printf("%s", del_item());
	} else {
		print_error_qfile_missing(self);
	}
}
