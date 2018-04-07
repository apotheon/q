#include "cli.h"

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

/* probably not meaningfully testable */
bool print_help(char *self) {
	cfreeprint(usage_text(self), LINESIZE);
	puts(help_text());
	return true;
}

char *help_text() {
	char *text = (
		"\n"
		"This queuer is a simple queue manager.  You can add things to the\n"
		"end of the queue.  You can pop things off the beginning.  You can\n"
		"move the first item to the end of the queue.  What more could you\n"
		"possibly need?  It provides the simplest way to intelligently\n"
		"manage a todo list, and simplicity is virtuous.  Of course, it\n"
		"also lets you cheat a little bit, because you can view the whole\n"
		"queue and delete items out of the middle of it, but only because\n"
		"people are imperfect and it would be nice to have simple ways to\n"
		"correct our mistakes.  A virtuous queuer user will not abuse these\n"
		"features.  With great power comes great responsibility.\n"
		"\n"
		"SYNTAX: q [command [argument]]\n"
		"\n"
		"COMMANDS:\n"
		"\n"
		"Running q with no <command> does the same thing as \"show\".\n"
		"\n"
		"In general, the longer the command, the less you should use it.\n"
		"\n"
		"Anything in brackets is optional.  Thus, you can use \"h\" for help\n"
		"and \"rot\" for rotate, or you can use \"help\" and \"rotate\".\n"
		"\n"
		"    h[elp]                  display this help message\n"
		"\n"
		"    add <item>              add item to end of queue\n"
		"\n"
		"    del                     delete top item from queue\n"
		"                            and display deleted item\n"
		"\n"
		"    list-all                display entire queue, numbered\n"
		"\n"
		"    remove-number <num>     remove item in position <num>\n"
		"\n"
		"    rot[ate]                move first item to last position\n"
		"                            and display new top item in queue\n"
		"\n"
		"    [show]                  display top item in queue\n"
		"\n"
		"    create-fresh-queue      set up queuer for use\n"
	);

	return text;
}

char *usage_text(char *self) {
	char *text = calloc(LINESIZE, sizeof(*text));
	check_alloc(text);

	const char *endtext = " <command> [argument]";

	strlcpy(text, "USAGE: ", LINESIZE);
	strlcat(text, self, LINESIZE);
	strlcat(text, endtext, LINESIZE);

	return text;
}

char *try_text(char *self) {
	char *text = calloc(LINESIZE, sizeof(*text));
	check_alloc(text);

	strlcpy(text, "Try \"", LINESIZE);
	strlcat(text, self, LINESIZE);
	strlcat(text, " help\" for more info.", LINESIZE);

	return text;
}

void cmd_with_arg(int count, char **arguments, char *cmd) {
	char *program = *(arguments);
	char *input = *(arguments + 2);

	if (match_cmd(cmd, "add")) add_item(input, program);
	else if (match_cmd(cmd, "remove-number")) remove_item_number(0);
	else print_invalid_command_line(count, arguments);
}

/* maybe test by executing "q add" command */
void print_invalid_command_line(int count, char **arguments) {
	char *self = *(arguments);

	printf("Command or argument(s) invalid: %s", self);
	for (int i = 1; i < count; ++i) printf(" %s", *(arguments + i));
	puts("");

	cfreeprint(usage_text(self), LINESIZE);
	cfreeprint(try_text(self), LINESIZE);
}

/* probably not meaningfully testable */
void print_error_empty() {
	perror("Error reading from queuefile (it may be empty)");
}

/* maybe test by executing "q create-fresh-queue" with an existing queue */
void print_error_exists(char *dir, char *q) {
	char *home = getenv("HOME");
	printf("The file \"%s/%s/%s\" already exists.\n", home, dir, q);
}

/* probably not meaningfully testable */
void print_error_open() {
	perror("Error opening queuefile.");
}

/* probably not meaningfully testable */
void print_error_qfile_missing(char *self) {
	printf("No queuefile found.  Try `%s create-fresh-queue`.\n", self);
}

void not_implemented(char *cmd) {
	printf("Command \"%s\" was not yet implemented.\n", cmd);
}
