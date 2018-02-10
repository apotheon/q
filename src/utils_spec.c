#include "utils.h"
#include "bdd-for-c.h"

spec("Utils") {
	static char *dirname = "test_directory";

	describe("exists()") {
		after() {
			if (exists(dirname)) rmdir(dirname);
		}

		before() {
			if (! exists(dirname)) newdir(dirname);
		}

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
