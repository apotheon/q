#include "errno.h"
#include "string.h"
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

char *del_line(char *fname) {
	char *tmp_file = calloc(LINESIZE, sizeof(*tmp_file));
	check_alloc(tmp_file);

	strlcpy(tmp_file, "tempq.XXXXXXXXXXXXX", LINESIZE);

	if (mkstemp(tmp_file) > 0) {
		unlink(tmp_file);
	} else {
		perror("no tempfile");
		exit(EXIT_FAILURE);
	}

	char *first_line = calloc(LINESIZE, sizeof(*first_line));
	check_alloc(first_line);

	FILE *ofile = fopen(fname, "r");
	FILE *tfile = fopen(tmp_file, "a");

	fgets(first_line, LINESIZE, ofile);

	char *line = calloc(LINESIZE, sizeof(*line));
	check_alloc(line);

	while (fgets(line, LINESIZE, ofile)) fprintf(tfile, "%s", line);

	clearfree(line, LINESIZE);
	fclose(ofile);
	fclose(tfile);

	if (rename(tmp_file, fname) == 0) {
		clearfree(tmp_file, LINESIZE);
		return first_line;
	} else {
		clearfree(tmp_file, LINESIZE);
		perror("failed to edit file");
		puts("Attempted changes may be found in tmpfile.");
		exit(EXIT_FAILURE);
	}
}

/* probably not meaningfully testable */

void clearfree(char *text, size_t text_size) {
	for (unsigned long i = 0; i < text_size; ++i) *(text + 1) = '\0';
	free(text);
}

void clearprint(char *text, size_t text_size) {
	puts(text);
	clearfree(text, text_size);
}
