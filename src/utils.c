#include "utils.h"

bool cd(char *dir) {
	if (chdir(dir) == 0) return true;
	else return false;
}

bool cd_qdir() {
	int success = false;
	char *home = getenv("HOME");

	if (cd(home)) {
		if (! exists(DIRNAME)) newdir(DIRNAME);
		success = cd(DIRNAME);
	}

	return success;
}

/* maybe prevent allocation somehow while executing `q list-all` to test */
bool check_alloc(char *p) {
	if (p == NULL) {
		free(p);
		perror("Failed to initialize a pointer.\n");
		exit(EXIT_FAILURE);
	} else {
		return true;
	}
}

bool exists(char *fname) {
	struct stat st = {0};

	if (stat(fname, &st) == 0) return true;
	else return false;
}

/* execute `q list-all` to test */
bool get_line(char *line, FILE *qfile) {
	if (fgets(line, LINESIZE - 1, qfile) > 0) return true;
	else return false;
}

bool newdir(char *dir) {
	if (mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0) return true;
	else return false;
}

bool qexists() {
	return (cd_qdir() && exists(QNAME));
}

char *del_line(unsigned int itemno, char *fname) {
	if (itemno == 1) {
		char *tmp_file = calloc(LINESIZE, sizeof(*tmp_file));
		check_alloc(tmp_file);

		char *first_line = calloc(LINESIZE, sizeof(*first_line));
		check_alloc(first_line);

		set_tempname(tmp_file);

		FILE *ofile = fopen(fname, "r");
		FILE *tfile = fopen(tmp_file, "a");

		char *line = calloc(LINESIZE, sizeof(*line));
		check_alloc(line);

		fgets(first_line, LINESIZE, ofile);
		while (fgets(line, LINESIZE, ofile)) fprintf(tfile, "%s", line);

		cfree(line, LINESIZE);
		fclose(ofile);
		fclose(tfile);

		if (rename(tmp_file, fname) != 0) {
			cfree(tmp_file, LINESIZE);
			perror("failed to edit file");
			puts("Attempted changes may be found in tmpfile.");
			exit(EXIT_FAILURE);
		}

		cfree(tmp_file, LINESIZE);
		return first_line;
	} else {
		puts("Command \"remove-item\" was not yet implemented.");
		exit(EXIT_SUCCESS);
	}
}

/* probably not meaningfully testable */

void cfree(char *text, size_t text_size) {
	for (unsigned long i = 0; i < text_size; ++i) *(text + i) = '\0';
	free(text);
}

void clearfree(size_t text_size, int argnum, ...) {
	va_list arglist;
	va_start(arglist, argnum);
	for (int n = 0; n < argnum; ++n) cfree(va_arg(arglist, char *), text_size);
	va_end(arglist);
}

void cfreeprint(char *text, size_t text_size) {
	puts(text);
	cfree(text, text_size);
}

void set_tempname(char *tmp_file) {
	strlcpy(tmp_file, "tempq.XXXXXXXXXXXXX", LINESIZE);

	if (mkstemp(tmp_file) > 0) {
		unlink(tmp_file);
	} else {
		perror("no tempfile");
		exit(EXIT_FAILURE);
	}
}
