#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define LINESIZE 1000000

typedef enum { false, true } bool;
char *dirname = ".quebert";
char *qname = "queue.txt";

bool cd();
bool cd_qdir();
bool exists(char *fname);
bool get_line(char *line, FILE *qfile);
bool help();
bool match_cmd(char *cmd, char *cmdtarget);
bool match_cmd_add_remove(char *cmd);
bool match_cmd_with_arg(char *cmd, int argc);
bool match_help(char *cmd);
bool match_rot(char *cmd);
bool missing(char *fname);
bool newdir(char *dir);
bool qexists();
bool usage(char *self);

void add_item(char *input);
void cmd_with_arg(int argc, char **argv, char *cmd);
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
void try_help();

int main(int argc, char **argv) {
	char *program = *(argv);

	if (argc > 1) {
		char *cmd = *(argv + 1);

		if (match_help(cmd)) {
			usage(program) && help();
		} else if (match_cmd_with_arg(cmd, argc)) {
			cmd_with_arg(argc, argv, cmd);
		} else if (match_cmd(cmd, "create-fresh-queue")) {
			start_queuer();
		} else if (match_cmd(cmd, "del")) {
			not_implemented(cmd);
		} else if (match_cmd(cmd, "list-all")) {
			list_all();
		} else if (match_cmd(cmd, "show")) {
			show_head();
		} else if (match_rot(cmd)) {
			not_implemented(cmd);
		} else if (match_cmd_add_remove(cmd)) {
			printf("Command \"%s\" requires exactly one argument.", cmd);
			usage(program);
			try_help(program);
		} else {
			printf("Command \"%s\" is not valid.\n", cmd);
			usage(program);
			try_help(program);
		}
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
		if (missing(dirname)) newdir(dirname);
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

bool match_cmd_add_remove(char *cmd) {
	return match_cmd(cmd, "add") || match_cmd(cmd, "remove-number");
}

bool match_cmd_with_arg(char *cmd, int argc) {
	return match_cmd_add_remove(cmd) && argc == 3;
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

bool missing(char *fname) {
	return ! exists(fname);
}

bool newdir(char *dir) {
	if (mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0) return true;
	else return false;
}

bool qexists() {
	return (cd_qdir() && exists(qname));
}

bool usage(char *self) {
	if (printf("%s <command> [argument]\n", self)) return true;
	else return false;
}

bool help() {
	puts("");
	puts("This queuer is a simple queue manager.  You can add things to the");
	puts("end of the queue.  You can pop things off the beginning.  You can");
	puts("move the first item to the end of the queue.  What more could you");
	puts("possibly need?  It provides the simplest way to intelligently");
	puts("manage a todo list, and simplicity is virtuous.  Of course, it");
	puts("also lets you cheat a little bit, because you can view the whole");
	puts("queue and delete items out of the middle of it, but only because");
	puts("people are imperfect and it would be nice to have simple ways to");
	puts("correct our mistakes.  A virtuous queuer user will not abuse these");
	puts("features.  With great power comes great responsibility.");
	puts("");
	puts("SYNTAX: q [command [argument]]");
	puts("Running q with no <command> does the same thing as \"show\".");
	puts("");
	puts("COMMANDS:");
	puts("");
	puts("In general, the longer the command, the less you should use it.");
	puts("Anything in brackets is optional.  Thus, you can use \"h\" for help");
	puts("and \"rot\" for rotate, or you can use \"help\" and \"rotate\".");
	puts("");
	puts("    h[elp]                  display this help message");
	puts("    add <item>              add item to end of queue");
	puts("    del                     delete top item from queue");
	puts("    list-all                display complete queue, numbered");
	puts("    remove-number <num>     remove item in position <num>");
	puts("    rot[ate]                move first item to last position");
	puts("    [show]                  show top item in queue");
	puts("    create-fresh-queue      set up queuer for use");
	puts("");

	return true;
}

void cmd_with_arg(int argc, char **argv, char *cmd) {
	char *input = *(argv + 2);

	if (match_cmd(cmd, "add")) add_item(input);
	else if (match_cmd(cmd, "remove-number")) remove_item_number(cmd);
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
	char *line = (char*) malloc(LINESIZE);
	bool next = false;

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

void try_help(char *self) {
	printf("Try \"%s help\" for more info.\n", self);
}
