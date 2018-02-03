#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define LINESIZE 1000000

typedef enum { false, true } bool;

int add();
int cd();
int cd_qdir();
int exists();
int help();
int list_all();
int match_cmd();
int match_help();
int match_rot();
int not_implemented();
int print_empty_queuefile();
int print_queuefile_exists();
int print_no_queuefile();
int show_head();
int start_queuer();
int try_help();
int usage();

int main(int argc, char **argv) {
	char *program = *(argv);

	if (argc > 1) {
		char *cmd = *(argv + 1);
		char *input = *(argv + 2);

		if (match_help(cmd)) usage(program) && help();
		else if (match_cmd(cmd, "add")) add(input);
		else if (match_cmd(cmd, "del")) not_implemented(cmd);
		else if (match_cmd(cmd, "list-all")) list_all();
		else if (match_cmd(cmd, "remove-number")) not_implemented(cmd);
		else if (match_cmd(cmd, "show")) show_head();
		else if (match_cmd(cmd, "create-fresh-queue")) start_queuer();
		else if (match_rot(cmd)) not_implemented(cmd);
		else {
			printf("Command \"%s\" is not valid.\n", cmd);
			usage(program);
			try_help(program);
		}
	} else {
		show_head();
	}

	return 0;
}

int usage(char *self) {
	printf("%s <command> [argument]\n", self);
	return 1;
}

int try_help(char *self) {
	printf("Try \"%s help\" for more info.\n", self);
	return 1;
}

int newdir(char *dirname) {
	return mkdir(dirname, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

int match_cmd(char *cmd, char *cmdtarget) {
	int l = strnlen(cmd, 1000);

	if (!strncmp(cmd, cmdtarget, l)) return 1;
	else return 0;
}

int match_help(char *cmd) {
	return (
		match_cmd(cmd, "h") ||
		match_cmd(cmd, "help") ||
		match_cmd(cmd, "-h") ||
		match_cmd(cmd, "--help")
	);
}

int match_rot(char *cmd) {
	return (match_cmd(cmd, "rot") || match_cmd(cmd, "rotate"));
}

int not_implemented(char *cmd) {
	printf("Command \"%s\" was not yet implemented.\n", cmd);
	return 0;
}

int exists(char *fname) {
	struct stat st = {0};
	return !stat(fname, &st);
}

int cd(char *dirname) {
	return !chdir(dirname);
}

int cd_qdir() {
	char *home = getenv("HOME");
	return (cd(home) && cd(".quebert"));
}

int add(char *input) {
	not_implemented("add");
	return 0;
}

int print_numbered_file_listing(char c, FILE *qfile) {
	bool new_item = false;
	int n = 1;

	printf("%4d ", n);

	while (c != EOF) {
		putchar(c);

		c = fgetc(qfile);

		if (new_item && (c != EOF)) {
			printf("%4d ", ++n);
			new_item = false;
		}

		if (c == '\n') new_item = true;
	}

	return 0;
}

int list_all() {
	char *queue = "queue.txt";

	if (cd_qdir() && exists(queue)) {
		FILE *qfile;
		qfile = fopen(queue, "r");

		char c;

		if (qfile == NULL) {
			perror("Error opening queuefile.");
		} else if ((c = fgetc(qfile))) {
			print_numbered_file_listing(c, qfile);
		} else {
			print_empty_queuefile();
		}

		fclose(qfile);
	} else {
		print_no_queuefile();
	}

	return 0;
}

int show_head() {
	char *queue = "queue.txt";

	if (cd_qdir() && exists(queue)) {
		FILE *qfile;
		qfile = fopen(queue, "r");

		char str[LINESIZE];

		if (qfile == NULL) {
			perror("Error opening queuefile.");
		} else if (fgets(str, LINESIZE - 1, qfile) != NULL) {
			printf("%s", str);
		} else {
			print_empty_queuefile();
		}

		fclose(qfile);
	} else {
		print_no_queuefile();
	}

	return 0;
}

int print_empty_queuefile() {
	perror("Error reading from queuefile (it may be empty)");
	return 0;
}

int print_queuefile_exists(char *home, char *dir, char *q) {
	printf("A file named \"%s/%s/%s\" already exists.\n", home, dir, q);
	return 0;
}

int print_no_queuefile() {
	puts("No queuefile found.  Create one with `q create-fresh-queue`.");
	return 0;
}

int start_queuer() {
	char *home = getenv("HOME");
	chdir(home);

	char *dirname = ".quebert";
	if (!exists(dirname)) newdir(dirname);
	chdir(dirname);

	char *qname = "queue.txt";
	if (exists(qname)) print_queuefile_exists(home, dirname, qname);
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
