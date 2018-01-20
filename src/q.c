#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int help();
int match_cmd();
int match_help();
int match_rot();
int not_implemented();
int show_head();
int start_queuer();
int try_help();
int usage();

int main(int argc, char **argv) {
	if (argc > 1) {
		char *program = *(argv);
		char *cmd = *(argv + 1);

		if (match_help(cmd)) usage(program) && help();
		else if (match_cmd(cmd, "add")) not_implemented(cmd);
		else if (match_cmd(cmd, "del")) not_implemented(cmd);
		else if (match_cmd(cmd, "list-all")) not_implemented(cmd);
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
	mkdir(dirname, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	return 0;
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

int show_head() {
	not_implemented("show");
	return 0;
}

int start_queuer() {
	char *home = getenv("HOME");

	struct stat st1 = {0};
	char *dirname = ".quebert";

	chdir(home);
	if (stat(dirname, &st1) == -1) newdir(dirname);

	struct stat st2 = {0};
	char *qname = "queue.txt";

	chdir(dirname);
	if (stat(qname, &st2) == -1) open(qname, O_CREAT, 0600);
	else printf(
		"A file named \"%s/%s/%s\" already exists.\n", home, dirname, qname
	);

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
