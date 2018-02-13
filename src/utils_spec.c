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
			it("changes directory") {
				char *original_dir = calloc(PATH_MAX, sizeof(*original_dir));
				check_alloc(original_dir);

				getcwd(original_dir, PATH_MAX);

				check(cd(dirname));

				cd(original_dir);
				free(original_dir);
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

		it("creates a new directory") {
			check(! exists(dirname));
			check(newdir(dirname) && exists(dirname));
		}
	}
}
