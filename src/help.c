#include "help.h"

char * help_text() {
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
		"Running q with no <command> does the same thing as \"show\".\n"
		"\n"
		"COMMANDS:\n"
		"\n"
		"In general, the longer the command, the less you should use it.\n"
		"Anything in brackets is optional.  Thus, you can use \"h\" for help\n"
		"and \"rot\" for rotate, or you can use \"help\" and \"rotate\".\n"
		"\n"
		"    h[elp]                  display this help message\n"
		"    add <item>              add item to end of queue\n"
		"    del                     delete top item from queue\n"
		"    list-all                display complete queue, numbered\n"
		"    remove-number <num>     remove item in position <num>\n"
		"    rot[ate]                move first item to last position\n"
		"    [show]                  show top item in queue\n"
		"    create-fresh-queue      set up queuer for use\n"
		"\n"
	);

	return text;
}

bool print_help(char *self) {
	clearprint(usage_text(self));
	puts(help_text());
	return true;
}

char * usage_text(char *self) {
	char *text = calloc(LINESIZE, sizeof(*text));
	check_alloc(text);

	const char *endtext = " <command> [argument]";

	strlcpy(text, "USAGE: ", LINESIZE);
	strlcat(text, self, LINESIZE);
	strlcat(text, endtext, LINESIZE);

	return text;
}

char * try_text(char *self) {
	char *text = calloc(LINESIZE, sizeof(*text));
	check_alloc(text);

	const char *endtext = " help\" for more info.";

	strlcpy(text, "Try \"", LINESIZE);
	strlcat(text, self, LINESIZE);
	strlcat(text, endtext, LINESIZE);

	return text;
}
