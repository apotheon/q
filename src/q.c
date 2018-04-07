#include "config.h"
#include "globals.h"
#include "cli.h"
#include "features.h"
#include "utils.h"

int main(int argc, char **argv) {
	char *program = *(argv);

	if (argc > 1) {
		char *cmd = *(argv + 1);

		if (match_help(cmd)) print_help(program);
		else if (argc > 2) cmd_with_arg(argc, argv, cmd);
		else if (match_cmd(cmd, "create-fresh-queue")) start_queuer();
		else if (match_cmd(cmd, "del")) del(program);
		else if (match_cmd(cmd, "list-all")) list_all(program);
		else if (match_cmd(cmd, "show")) show_head(program);
		else if (match_rot(cmd)) rot(program);
		else print_invalid_command_line(argc, argv);
	} else {
		show_head(program);
	}

	return 0;
}
