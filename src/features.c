#include "features.h"

char *del_item(uint16_t itemno) {
	return del_line(itemno, QNAME);
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
	if (qexists()) {
		printf("deleted: %s", del_item(1));
		show_head(self);
	} else {
		print_error_qfile_missing(self);
	}
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
	int next = 0;

	if ((next = fgetc(qfile)) == EOF) {
		print_error_empty();
	} else {
		while (next != EOF) {
			printf("%4d ", ++n);

			while ((next != '\n') && (next != EOF)) {
				putchar(next);
				next = fgetc(qfile);
			}
		}
	}
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
	char *rot_string = del_item(1);
	chomp(rot_string);
	add_item(rot_string, self);
}

void show_head(char *self) {
	if (qexists()) {
		FILE *qfile = fopen(QNAME, "r");

		int next = 0;

		if (! qfile) {
			print_error_open();
		} else if ((next = fgetc(qfile)) == EOF) {
			print_error_empty();
		} else {
			while ((next != EOF) && (next != '\n')) {
				putchar(next);
				next = fgetc(qfile);
			}
		}

		fclose(qfile);
	} else {
		print_error_qfile_missing(self);
	}
}

void start_queuer() {
	cd_qdir();

	if (qexists()) print_error_exists(DIRNAME, QNAME);
	else open(QNAME, O_CREAT, 0600);
}
