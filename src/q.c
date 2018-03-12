#include <fcntl.h>
#include "config.h"
#include "globals.h"
#include "cli.h"
#include "features.h"
#include "utils.h"

void add_item(char *input, char *self);
void cmd_with_arg(int argc, char **argv, char *cmd);
void list_all();
void not_implemented(char *cmd);
void print_numbered_file_listing(FILE *qfile);
void remove_item_number(char *cmd);
void show_head();
void start_queuer();

int main(int argc, char **argv) {
	char *program = *(argv);

	if (argc > 1) {
		char *cmd = *(argv + 1);

		if (match_help(cmd)) print_help(program);
		else if (argc > 2) cmd_with_arg(argc, argv, cmd);
		else if (match_cmd(cmd, "create-fresh-queue")) start_queuer();
		else if (match_cmd(cmd, "del")) del(program);
		else if (match_cmd(cmd, "list-all")) list_all();
		else if (match_cmd(cmd, "show")) show_head();
		else if (match_rot(cmd)) not_implemented(cmd);
		else print_invalid_command_line(argc, argv);
	} else {
		show_head();
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

void list_all(char *self) {
	if (qexists()) {
		FILE *qfile = fopen(QNAME, "r");

		if (! qfile) print_error_open();
		else print_numbered_file_listing(qfile);

		fclose(qfile);
	} else {
		print_error_qfile_missing(self);
	}
}

void show_head(char *self) {
	if (qexists()) {
		FILE *qfile = fopen(QNAME, "r");

		char *line = calloc(LINESIZE, sizeof(*line));
		check_alloc(line);

		if (! qfile) print_error_open();
		else if (get_line(line, qfile)) printf("%s", line);
		else print_error_empty();

		fclose(qfile);
		cfree(line, LINESIZE);
	} else {
		print_error_qfile_missing(self);
	}
}

void not_implemented(char *cmd) {
	printf("Command \"%s\" was not yet implemented.\n", cmd);
}

void print_numbered_file_listing(FILE *qfile) {
	int n = 0;
	bool next = false;

	char *line = calloc(LINESIZE, sizeof(*line));
	check_alloc(line);

	if ((next = get_line(line, qfile))) {
		while (next) {
			printf("%4d %s", ++n, line);
			next = get_line(line, qfile);
		}
	} else {
		print_error_empty();
	}

	cfree(line, LINESIZE);
}

void remove_item_number(char *cmd) {
	not_implemented(cmd);
}

void start_queuer() {
	cd_qdir();

	if (qexists()) print_error_exists(DIRNAME, QNAME);
	else open(QNAME, O_CREAT, 0600);
}
