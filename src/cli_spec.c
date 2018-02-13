#include "cli.h"
#include "bdd-for-c.h"

spec("CLI") {
	describe("help_text()") {
		it("should return help text") {
			char *help_text_string = "display this help message";
			check(strstr(help_text(), help_text_string));
		}
	}

	describe("match_cmd()") {
		char *target_string = "remove-number";
		int cmdsize = sizeof(*target_string);

		it("should return true with a command match") {
			char *cmd = calloc(LINESIZE, cmdsize);
			strlcpy(cmd, target_string, cmdsize);
			check(match_cmd(cmd, target_string) == true);
		}

		it("should return false with a command mismatch") {
			char *cmd = "remove-numbers";
			check(match_cmd(cmd, target_string) == false);
		}
	}

	describe("match_help()") {
		it("should return false with incorrect help command") {
			check(match_help("help-me") == false);
		}

		it("should return false with incorrect help option") {
			check(match_help("-help") == false);
		}

		it("should return false with non-help command") {
			check(match_help("add") == false);
		}

		it("should return true with correct help commands") {
			check(match_help("h") && match_help("help"));
		}

		it("should return true with correct help options") {
			check(match_help("-h") && match_help("--help"));
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
