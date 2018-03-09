#include <limits.h>
#include "utils.h"
#include "test_helpers.h"
#include "features.h"
#include "bdd-for-c.h"

spec("Features") {
	context("with queuefile containing todo items") {
		FILE *qfile;

		char *line = calloc(LINESIZE, sizeof(*line));
		check_alloc(line);

		before_each() {
			prep_testq();
		}

		after_each() {
			cleanup_testq();
			fclose(qfile);
			cfree(line, LINESIZE);
		}

		describe("add_item()") {
			it("adds a line at the end of the queuefile") {
			}
		}

		describe("del_item()") {
			it("deletes the first line in the queuefile") {
				char *deleted = del_item();
				qfile = fopen(QNAME, "r");

				if (! qfile) {
					fclose(qfile);
					clearfree(LINESIZE, 2, deleted, line);
					exit(EXIT_FAILURE);
				} else if (get_line(line, qfile)) {
					check(strncmp(deleted, "FIRST LINE\n", LINESIZE) == 0);
					check(strncmp(line, "SECOND LINE\n", LINESIZE) == 0);
				} else {
					clearfree(LINESIZE, 2, deleted, line);
					exit(EXIT_FAILURE);
				}

				cfree(deleted, LINESIZE);
			}
		}
	}
}
