#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "globals.h"
#include "help.h"

char *dirname = ".quebert";
char *qname = "queue.txt";

bool cd(char *dir);
bool cd_qdir();
bool exists(char *fname);
bool get_line(char *line, FILE *qfile);
bool match_cmd(char *cmd, char *cmdtarget);
bool match_help(char *cmd);
bool match_rot(char *cmd);
bool newdir(char *dir);
bool qexists();

void add_item(char *input);
void cmd_with_arg(int argc, char **argv, char *cmd);
int del_item();
void invalid_command_line(char *program);
void list_all();
void not_implemented(char *cmd);
void print_error_empty();
void print_error_exists(char *dir, char *q);
void print_error_open();
void print_error_qfile_missing();
void print_numbered_file_listing(FILE *qfile);
void remove_item_number(char *cmd);
void show_head();
void start_queuer();

int main(int argc, char **argv) {
	char *program = *(argv);

	if (argc > 1) {
		char *cmd = *(argv + 1);

		if (match_help(cmd)) usage(program) && print_help();
		else if (argc > 2) cmd_with_arg(argc, argv, cmd);
		else if (match_cmd(cmd, "create-fresh-queue")) start_queuer();
		else if (match_cmd(cmd, "del")) del_item();
		else if (match_cmd(cmd, "list-all")) list_all();
		else if (match_cmd(cmd, "show")) show_head();
		else if (match_rot(cmd)) not_implemented(cmd);
		else invalid_command_line(program);
	} else {
		show_head();
	}

	return 0;
}

  /*******************/
 /* OTHER FUNCTIONS */
/*******************/

bool cd(char *dir) {
	if (chdir(dir) == 0) return true;
	else return false;
}

bool cd_qdir() {
	int success = false;
	char *home = getenv("HOME");

	if (cd(home)) {
		if (! exists(dirname)) newdir(dirname);
		success = cd(dirname);
	}

	return success;
}

bool exists(char *fname) {
	struct stat st = {0};

	if (stat(fname, &st) == 0) return true;
	else return false;
}

bool get_line(char *line, FILE *qfile) {
	if (fgets(line, LINESIZE - 1, qfile) > 0) return true;
	else return false;
}

bool match_cmd(char *cmd, char *cmdtarget) {
	if (strncmp(cmd, cmdtarget, strnlen(cmd, LINESIZE)) == 0) return true;
	else return false;
}

bool match_help(char *cmd) {
	return (
		match_cmd(cmd, "--help") || match_cmd(cmd, "-h") ||
		match_cmd(cmd, "help") || match_cmd(cmd, "h")
	);
}

bool match_rot(char *cmd) {
	return (match_cmd(cmd, "rot") || match_cmd(cmd, "rotate"));
}

bool newdir(char *dir) {
	if (mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0) return true;
	else return false;
}

bool qexists() {
	return (cd_qdir() && exists(qname));
}

void add_item(char *input) {
	if (qexists()) {
		FILE *qfile = fopen(qname, "a");

		if (! qfile) print_error_open();
		else fprintf(qfile, "%s\n", input);

		fclose(qfile);
	} else {
		print_error_qfile_missing();
	}
}

void cmd_with_arg(int argc, char **argv, char *cmd) {
	char *input = *(argv + 2);

	if (match_cmd(cmd, "add")) add_item(input);
	else if (match_cmd(cmd, "remove-number")) remove_item_number(cmd);
	else invalid_command_line(*argv);
}

int del_item() {
	if (qexists()) {
		FILE *qfile = fopen(qname, "r");

		if (! qfile) {
			print_error_open();
		} else {
			int lnsize = LINESIZE - 1;
			char *tmp_file = (char*) malloc(LINESIZE);
			memset(tmp_file, 0, LINESIZE);
			strlcpy(tmp_file, "/tmp/tempq.XXXXXXXXXXXXX", lnsize);

			if (mkstemp(tmp_file) > 0) {
				unlink(tmp_file);
			} else {
				puts("No tempfile.");
				return 0;
			}

			FILE *tfile = fopen(tmp_file, "a");

			char *line = (char*) malloc(LINESIZE);
			memset(line, 0, LINESIZE);

			fgets(line, lnsize, qfile);
			char *deleted = line;

			while (fgets(line, lnsize, qfile)) fprintf(tfile, "%s", line);
			fclose(tfile);

			tfile = fopen(tmp_file, "r");
			while (fgets(line, lnsize, tfile)) fprintf(qfile, "%s", line);
			fclose(tfile);
		}

		fclose(qfile);
	} else {
		print_error_qfile_missing();
	}

	return 0;
}

void invalid_command_line(char *self) {
	puts("Command or argument(s) invalid.");
	usage(self);
	try_help(self);
}

void list_all() {
	if (qexists()) {
		FILE *qfile = fopen(qname, "r");

		if (! qfile) print_error_open();
		else print_numbered_file_listing(qfile);

		fclose(qfile);
	} else {
		print_error_qfile_missing();
	}
}

void show_head() {
	if (cd_qdir() && exists(qname)) {
		FILE *qfile = fopen(qname, "r");
		char *line = (char*) malloc(LINESIZE);
		memset(line, 0, LINESIZE);

		if (! qfile) print_error_open();
		else if (get_line(line, qfile)) printf("%s", line);
		else print_error_empty();

		fclose(qfile);
	} else {
		print_error_qfile_missing();
	}
}

void not_implemented(char *cmd) {
	printf("Command \"%s\" was not yet implemented.\n", cmd);
}

void print_error_empty() {
	perror("Error reading from queuefile (it may be empty)");
}

void print_error_exists(char *dir, char *q) {
	char *home = getenv("HOME");
	printf("The file \"%s/%s/%s\" already exists.\n", home, dir, q);
}

void print_error_open() {
	perror("Error opening queuefile.");
}

void print_error_qfile_missing() {
	puts("No queuefile found.  Create one with `q create-fresh-queue`.");
}

void print_numbered_file_listing(FILE *qfile) {
	int n = 0;
	bool next = false;
	char *line = (char*) malloc(LINESIZE);
	memset(line, 0, LINESIZE);

	if ((next = get_line(line, qfile))) {
		while (next) {
			printf("%4d %s", ++n, line);
			next = get_line(line, qfile);
		}
	} else {
		print_error_empty();
	}
}

void remove_item_number(char *cmd) {
	not_implemented(cmd);
}

void start_queuer() {
	cd_qdir();

	if (exists(qname)) print_error_exists(dirname, qname);
	else open(qname, O_CREAT, 0600);
}
