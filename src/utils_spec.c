#include "utils.h"
#include "bdd-for-c.h"

spec("Utils") {
	describe("exists()") {
		it("returns true if a directory exists") {
			check(exists("."));
		}

		it("returns true if a file exists") {
			check(exists("a.out"));
		}

		it("returns false if a directory or file doesn't exist") {
			check(! exists("nonexistent_file.txt"));
		}
	}
}
