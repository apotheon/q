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
			char *usage_text_string_start = "USAGE:";
			char *usage_text_string_end = "<command> [argument]";
			check(strstr(usage_text(), usage_text_string_start));
			check(strstr(usage_text(), usage_text_string_end));
		}
	}
}
