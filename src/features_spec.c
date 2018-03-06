#include <limits.h>
#include "utils.h"
#include "test_helpers.h"
#include "features.h"
#include "bdd-for-c.h"

spec("Features") {
	describe("del_item()") {
		context("with queuefile containing todo items") {
			before_each() {
				prep_testq();
			}

			after_each() {
				cleanup_testq();
			}

			it("deletes the first line in the queuefile") {
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
			}
		}
	}
}
