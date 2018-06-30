#include "features.h"

const char *err_emptyq = "Error reading from queuefile (it may be empty)";
const char *err_openq = "Error opening queuefile.";

char *del_item(uint16_t itemno) {
	return del_line(itemno, QNAME);
}

void add_item(char *input, char *self) {
	if (qexists()) {
		FILE *qfile = fopen(QNAME, "a");

		if (! qfile) perror(err_openq);
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

		if (! qfile) perror(err_openq);
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
		perror(err_emptyq);
	} else {
		printf("%4d ", ++n);

		while (next != EOF) {
			putchar(next);

			if ((next == '\n') && ((next = fgetc(qfile)) != EOF)) {
				printf("%4d ", ++n);
			} else {
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
			perror(err_openq);
		} else if ((next = fgetc(qfile)) == EOF) {
			perror(err_emptyq);
		} else {
			while ((next != EOF) && (next != '\n')) {
				putchar(next);
				next = fgetc(qfile);
			}

			puts("");
		}

		fclose(qfile);
	} else {
		print_error_qfile_missing(self);
	}
}

void start_queuer() {
	cd_qdir();

	if (qexists()) print_error_qfile_exists(DIRNAME, QNAME);
	else open(QNAME, O_CREAT, 0600);
}
