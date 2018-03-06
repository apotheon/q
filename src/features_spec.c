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

				char *deleted = del_item();

				qfile = fopen(QNAME, "r");

				if (! qfile) {
					fclose(qfile);
					cfree(cwd, PATH_MAX);
					clearfree(LINESIZE, 2, deleted, line);
					exit(EXIT_FAILURE);
				} else if (get_line(line, qfile)) {
					check(strncmp(deleted, "FIRST LINE\n", LINESIZE) == 0);
					check(strncmp(line, "SECOND LINE\n", LINESIZE) == 0);
				} else {
					puts(cwd);
					cfree(cwd, PATH_MAX);
					clearfree(LINESIZE, 2, deleted, line);
					exit(EXIT_FAILURE);
				}

				fclose(qfile);
				cfree(cwd, PATH_MAX);
				clearfree(LINESIZE, 2, deleted, line);
			}
		}
	}
}
