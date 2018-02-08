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

bool exists(char *fname);
bool get_line(char *line, FILE *qfile);
bool match_cmd(char *cmd, char *cmdtarget);
bool match_help(char *cmd);
bool match_rot(char *cmd);
bool qexists();

int add_item(char *input);
int cd();
int cd_qdir();
int help();
int list_all();
int not_implemented(char *cmd);
int print_error_empty();
int print_error_exists(char *dir, char *q);
int print_error_open();
int print_error_qfile_missing();
int print_numbered_file_listing(FILE *qfile);
int show_head();
int start_queuer();
int try_help();
int usage(char *self);

int main(int argc, char **argv) {
	char *program = *(argv);

	if (argc > 1) {
		char *cmd = *(argv + 1);

		if (match_help(cmd)) {
			usage(program) && help();
		} else if (argc > 2) {
			char *input = *(argv + 2);
			if (match_cmd(cmd, "add")) add_item(input);
			else if (match_cmd(cmd, "remove-number")) not_implemented(cmd);
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
		} else if (match_cmd(cmd, "add")) {
			puts("Command \"add\" requires an argument.");
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

bool exists(char *fname) {
	struct stat st = {0};

	if (!stat(fname, &st)) return true;
	else return false;
}

bool get_line(char *line, FILE *qfile) {
	if (fgets(line, LINESIZE - 1, qfile)) return true;
	else return false;
}

bool match_cmd(char *cmd, char *cmdtarget) {
	int l = strnlen(cmd, LINESIZE);

	if (!strncmp(cmd, cmdtarget, l)) return true;
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

bool qexists() {
	return (cd_qdir() && exists(qname));
}

int usage(char *self) {
	printf("%s <command> [argument]\n", self);
	return 1;
}

int try_help(char *self) {
	printf("Try \"%s help\" for more info.\n", self);
	return 1;
}

int newdir(char *dir) {
	return mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

int not_implemented(char *cmd) {
	printf("Command \"%s\" was not yet implemented.\n", cmd);
	return 0;
}

int cd(char *dir) {
	return !chdir(dir);
}

int cd_qdir() {
	int success = 0;
	char *home = getenv("HOME");

	if (cd(home)) {
		if (!exists(dirname)) newdir(dirname);
		success = cd(dirname);
	}

	return success;
}

int print_error_empty() {
	perror("Error reading from queuefile (it may be empty)");
	return 0;
}

int print_error_exists(char *dir, char *q) {
	char *home = getenv("HOME");
	printf("The file \"%s/%s/%s\" already exists.\n", home, dir, q);
	return 0;
}

int print_error_open() {
	perror("Error opening queuefile.");
	return 0;
}

int print_error_qfile_missing() {
	puts("No queuefile found.  Create one with `q create-fresh-queue`.");
	return 0;
}

int add_item(char *input) {
	if (qexists()) {
		FILE *qfile = fopen(qname, "a");

		if (! qfile) print_error_open();
		else fprintf(qfile, "%s\n", input);

		fclose(qfile);
	} else {
		print_error_qfile_missing();
	}

	return 0;
}

int list_all() {
	if (qexists()) {
		FILE *qfile = fopen(qname, "r");

		if (! qfile) print_error_open();
		else print_numbered_file_listing(qfile);

		fclose(qfile);
	} else {
		print_error_qfile_missing();
	}

	return 0;
}

int show_head() {
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

	return 0;
}

int print_numbered_file_listing(FILE *qfile) {
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

	return 0;
}

int start_queuer() {
	cd_qdir();

	if (exists(qname)) print_error_exists(dirname, qname);
	else open(qname, O_CREAT, 0600);

	return 0;
}

int help() {
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

	return 1;
}
