#include "features.h"

char *del_item() {
	return del_line(QNAME);
}

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

void del(char *self) {
	if (qexists()) printf("%s", del_item());
	else print_error_qfile_missing(self);
}

void list_all(char *self) {
	if (qexists()) {
		FILE *qfile = fopen(QNAME, "r");

		if (! qfile) print_error_open();
		else print_numbered_file_listing(qfile);

		fclose(qfile);
	} else {
		print_error_qfile_missing(self);
	}
}

void print_numbered_file_listing(FILE *qfile) {
	int n = 0;
	bool next = false;

	char *line = calloc(LINESIZE, sizeof(*line));
	check_alloc(line);

	if ((next = get_line(line, qfile))) {
		while (next) {
			printf("%4d %s", ++n, line);
			next = get_line(line, qfile);
		}
	} else {
		print_error_empty();
	}

	cfree(line, LINESIZE);
}

void rot(char *self) {
	if (qexists()) {
		rot_item(self);
		show_head(self);
	} else {
		print_error_qfile_missing(self);
	}
}

void rot_item(char *self) {
	char *rot_string = del_item();
	size_t rot_size = strnlen(rot_string, LINESIZE) - 1;

	*(rot_string + rot_size) = '\0';
	add_item(rot_string, self);
}

void show_head(char *self) {
	if (qexists()) {
		FILE *qfile = fopen(QNAME, "r");

		char *line = calloc(LINESIZE, sizeof(*line));
		check_alloc(line);

		if (! qfile) print_error_open();
		else if (get_line(line, qfile)) printf("%s", line);
		else print_error_empty();

		fclose(qfile);
		cfree(line, LINESIZE);
	} else {
		print_error_qfile_missing(self);
	}
}

void start_queuer() {
	cd_qdir();

	if (qexists()) print_error_exists(DIRNAME, QNAME);
	else open(QNAME, O_CREAT, 0600);
}
