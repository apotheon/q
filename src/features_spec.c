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

		char *line1 = "FIRST LINE\n";
		char *line2 = "SECOND LINE\n";

		before_each() {
			prep_testq();
		}

		after_each() {
			cleanup_testq();
			cfree(line, LINESIZE);
		}

		describe("add_item()") {
			char *line3 = "THIRD LINE\n";
			size_t s_len = strnlen(line1, LINESIZE) + strnlen(line2, LINESIZE);

			it("adds a line at the end of the queuefile") {
				add_item(line3, "q");
				qfile = fopen(QNAME, "r");

				if (! qfile) {
					fclose(qfile);
					cfree(line, LINESIZE);
					exit(EXIT_FAILURE);
				} else if (! get_line(line, qfile)) {
					fclose(qfile);
					cfree(line, LINESIZE);
					exit(EXIT_FAILURE);
				} else {
					fseek(qfile, s_len, SEEK_SET);
					get_line(line, qfile);

					check(strncmp(line, line3, LINESIZE) == 0);

					fclose(qfile);
					cfree(line, LINESIZE);
				}
			}
		}

		describe("del_item()") {
			it("deletes the first line in the queuefile") {
				check(strncmp(del_item(), line1, LINESIZE) == 0);

				qfile = fopen(QNAME, "r");

				if (! qfile) {
					fclose(qfile);
					cfree(line, LINESIZE);
					exit(EXIT_FAILURE);
				} else if (! get_line(line, qfile)) {
					fclose(qfile);
					cfree(line, LINESIZE);
					exit(EXIT_FAILURE);
				} else {
					check(strncmp(line, line2, LINESIZE) == 0);
					fclose(qfile);
				}
			}
		}

		describe("rot_item()") {
			it("moves the first item in queue to end of the queue") {
				rot_item("q");

				qfile = fopen(QNAME, "r");

				if (! qfile) {
					fclose(qfile);
					cfree(line, LINESIZE);
					exit(EXIT_FAILURE);
				} else if (! get_line(line, qfile)) {
					fclose(qfile);
					cfree(line, LINESIZE);
					exit(EXIT_FAILURE);
				} else {
					char *fmtstr = "string \"%s\" doesn't match \"%s\"";
					int comparison = strncmp(line, line2, LINESIZE);

					check(comparison == 0, fmtstr, line, line2);

					fseek(qfile, strnlen(line2, LINESIZE), SEEK_SET);
					get_line(line, qfile);
					comparison = strncmp(line, line1, LINESIZE);

					check(comparison == 0, fmtstr, line, line1);
					fclose(qfile);
				}
			}
		}
	}

	context("with no queuefile") {
		before_each() {
			cleanup_testq();
		}

		after_each() {
			cleanup_testq();
		}

		describe("start_queuer()") {
			it("creates an empty queuefile") {
				char *line = calloc(LINESIZE, sizeof(*line));
				check_alloc(line);

				start_queuer();

				FILE *qfile = fopen(QNAME, "r");

				check(qfile);
				check(! get_line(line, qfile));

				fclose(qfile);
				cfree(line, LINESIZE);
			}
		}
	}
}
