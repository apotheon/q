#include <fcntl.h>
#include <libgen.h>
#include <limits.h>
#include "utils.h"
#include "bdd-for-c.h"

spec("Utils") {
	static char *dirname = "test_directory_name";
	static char *filename = "test_file_name";

	before() {
		cd(getenv("HOME"));
	}

	after() {
		cd(getenv("HOME"));
	}

	context("with an existing directory and file") {
		before_each() {
			cd(getenv("HOME"));
			if (! exists(dirname)) newdir(dirname);
			if (! exists(filename)) open(filename, O_CREAT, 0600);
		}

		after_each() {
			cd(getenv("HOME"));
			if (exists(dirname)) rmdir(dirname);
			if (exists(filename)) remove(filename);
		}

		describe("cd()") {
			it("changes directory to specified path") {
				char *cwd = calloc(PATH_MAX, sizeof(*cwd));
				check_alloc(cwd);

				check(cd(dirname));

				getcwd(cwd, PATH_MAX);
				check(strncmp(basename(cwd), dirname, PATH_MAX) == 0);
				clearfree(cwd, PATH_MAX);
			}
		}

		describe("exists()") {
			it("returns true if a directory exists") {
				check(exists(dirname));
			}

			it("returns true if a file exists") {
				check(exists(filename));
			}

			it("returns false if a directory or file doesn't exist") {
				check(! exists("nonexistent_file.txt"));
			}
		}
	}

	describe("cd_qdir()") {
		it("changes directory to qdir path") {
			char *cwd = calloc(PATH_MAX, sizeof(*cwd));
			check_alloc(cwd);

			check(cd_qdir());

			getcwd(cwd, PATH_MAX);
			check(strncmp(basename(cwd), DIRNAME, PATH_MAX) == 0);

			clearfree(cwd, PATH_MAX);
			if (exists(DIRNAME)) rmdir(DIRNAME);
		}
	}

	describe("newdir()") {
		before() {
			cd(getenv("HOME"));
			if (exists(dirname)) rmdir(dirname);
		}

		after() {
			cd(getenv("HOME"));
			if (exists(dirname)) rmdir(dirname);
		}

		it("creates a new directory") {
			check(! exists(dirname));
			check(newdir(dirname) && exists(dirname));
		}
	}

	describe("qexists()") {
		before_each() {
			cd(getenv("HOME"));
			if (! exists(DIRNAME)) newdir(DIRNAME);
			cd(DIRNAME);
		}

		after_each() {
			cd(getenv("HOME"));
			cd(DIRNAME);
			if (exists(QNAME)) remove(QNAME);
			cd(getenv("HOME"));
			if (exists(DIRNAME)) rmdir(DIRNAME);
		}

		context("with existing queuefile") {
			before_each() {
				if (! exists(QNAME)) open(QNAME, O_CREAT, 0600);
			}

			after_each() {
				if (exists(QNAME)) remove(QNAME);
			}

			it("returns true if queue exists") {
				check(qexists() == true);
			}
		}

		context("without existing queuefile") {
			before_each() {
				if (exists(QNAME)) remove(QNAME);
			}

			it("returns false if queue does not exist") {
				check(qexists() == false);
			}
		}
	}

	describe("del_line()") {
		context("with queuefile containing todo items") {
			before_each() {
				cd(getenv("HOME"));
				if (! exists(DIRNAME)) newdir(DIRNAME);
				cd(DIRNAME);
				if (exists(QNAME)) remove(QNAME);
				if (! exists(QNAME)) open(QNAME, O_CREAT, 0600);
				FILE *qfile = fopen(QNAME, "w");

				if (! qfile) {
					perror("Error opening file.\n");
					perror("FS may be full or unwritable.\n");
					exit(EXIT_FAILURE);
				} else {
					fprintf(qfile, "%s\n", "FIRST LINE\nSECOND LINE");
					fclose(qfile);
				}
			}

			after_each() {
				cd(getenv("HOME"));
				cd(DIRNAME);
				if (exists(QNAME)) remove(QNAME);
				cd(getenv("HOME"));
				if (exists(DIRNAME)) rmdir(DIRNAME);
			}

			it("deletes the first line of the queuefile") {
				FILE *qfile = fopen(QNAME, "r");

				char *line = calloc(LINESIZE, sizeof(*line));
				check_alloc(line);

				char *cwd = calloc(PATH_MAX, sizeof(*cwd));
				check_alloc(cwd);
				getcwd(cwd, PATH_MAX);

				if (! qfile) {
					exit(EXIT_FAILURE);
				} else if (! get_line(line, qfile)) {
					exit(EXIT_FAILURE);
				}

				fclose(qfile);

				char *deleted = del_line(QNAME);

				qfile = fopen(QNAME, "r");

				if (! qfile) {
					fclose(qfile);
					clearfree(cwd, PATH_MAX);
					clearfree(deleted, LINESIZE);
					clearfree(line, LINESIZE);
					exit(EXIT_FAILURE);
				} else if (get_line(line, qfile)) {
					check(strncmp(deleted, "FIRST LINE\n", LINESIZE) == 0);
					check(strncmp(line, "SECOND LINE\n", LINESIZE) == 0);
				} else {
					puts(cwd);
					clearfree(cwd, PATH_MAX);
					clearfree(deleted, LINESIZE);
					clearfree(line, LINESIZE);
					exit(EXIT_FAILURE);
				}

				fclose(qfile);
				clearfree(cwd, PATH_MAX);
				clearfree(deleted, LINESIZE);
				clearfree(line, LINESIZE);
			}
		}
	}
}
