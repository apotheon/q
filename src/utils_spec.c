#include <limits.h>
#include "utils.h"
#include "bdd-for-c.h"

spec("Utils") {
	static char *dirname = "test_directory_name";

	context("with an existing directory") {
		after() {
			if (exists(dirname)) rmdir(dirname);
		}

		before_each() {
			if (! exists(dirname)) newdir(dirname);
		}

		describe("cd()") {
			after() {
				char *current_dir = (char*) malloc(PATH_MAX + 1);
				getwd(current_dir);
				if (strstr(current_dir, dirname)) cd("..");
			}

			it("changes directory") {
				check(cd(dirname));
			}
		}

		describe("exists()") {
			it("returns true if a directory exists") {
				check(exists(dirname));
			}

			it("returns true if a file exists") {
				check(exists("a.out"));
			}

			it("returns false if a directory or file doesn't exist") {
				check(! exists("nonexistent_file.txt"));
			}
		}
	}

	describe("newdir()") {
		after() {
			if (exists(dirname)) rmdir(dirname);
		}

		before() {
			if (exists(dirname)) rmdir(dirname);
		}

		it("creates a newdirectory") {
			check(! exists(dirname));
			check(newdir(dirname) && exists(dirname));
		}
	}
}
