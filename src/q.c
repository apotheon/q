#include <fcntl.h>
#include "config.h"
#include "globals.h"
#include "cli.h"
#include "features.h"
#include "utils.h"

void cmd_with_arg(int argc, char **argv, char *cmd);
void not_implemented(char *cmd);
void remove_item_number(char *cmd);

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
		else if (match_rot(cmd)) not_implemented(cmd);
		else print_invalid_command_line(argc, argv);
	} else {
		show_head(program);
	}

	return 0;
}

  /*******************/
 /* OTHER FUNCTIONS */
/*******************/

void cmd_with_arg(int argc, char **argv, char *cmd) {
	char *program = *(argv);
	char *input = *(argv + 2);

	if (match_cmd(cmd, "add")) add_item(input, program);
	else if (match_cmd(cmd, "remove-number")) remove_item_number(cmd);
	else print_invalid_command_line(argc, argv);
}

void not_implemented(char *cmd) {
	printf("Command \"%s\" was not yet implemented.\n", cmd);
}

void remove_item_number(char *cmd) {
	not_implemented(cmd);
}
