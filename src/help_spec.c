#include "help.h"
#include "bdd-for-c.h"

spec("help") {
	it("should return help text") {
		char *help_help_text = "display this help message";
		check(strstr(help_text(), help_help_text));
	}
}
