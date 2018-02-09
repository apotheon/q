#include "help.h"
#include "bdd-for-c.h"

spec("Help") {
	describe("help_text()") {
		it("should return help text") {
			char *help_text_string = "display this help message";
			check(strstr(help_text(), help_text_string));
		}
	}

	describe("usage_text()") {
		it("should return usage text") {
			char *usage_text_string = "USAGE: q <command> [argument]";
			check(strcmp(usage_text("q"), usage_text_string) == 0);
		}
	}

	describe("try_text()") {
		it("should return try text") {
			char *try_text_string = "Try \"q help\" for more info.";
			check(strcmp(try_text("q"), try_text_string) == 0);
		}
	}
}
