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
				free(cwd);
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
		it("changes to directory to qdir path") {
			char *cwd = calloc(PATH_MAX, sizeof(*cwd));
			check_alloc(cwd);

			check(cd_qdir());

			getcwd(cwd, PATH_MAX);
			check(strncmp(basename(cwd), DIRNAME, PATH_MAX) == 0);

			free(cwd);
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
}
