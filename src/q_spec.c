#include "utils.h"
#include "test_helpers.h"
#include "features.h"
#include "bdd-for-c.h"

spec("queuer") {
	char *home = getenv("HOME");

	char *qpath = calloc(LINESIZE, sizeof(*qpath));
	check_alloc(qpath);
	snprintf(qpath, LINESIZE, "%s/%s/%s", home, DIRNAME, QNAME);

	char *output = calloc(LINESIZE, sizeof(*output));
	check_alloc(output);

	describe("add") {
		it("should error out when no todo string provided") {
			prep_testq();

			cd(getenv("HOME"));
			cd("..");

			char *err = (
				"Command or argument(s) invalid: ./q add\n"
				"USAGE: ./q <command> [argument]\n"
				"Try \"./q help\" for more info.\n"
			);

			FILE *qadd = popen("./q add 2>&1", "r");

			check(filestringcmp(qadd, err));
			pclose(qadd);

			cleanup_testq();
		}

		it("should error out with no existing queue") {
			check(test_no_queue_error("./q add foobar 2>&1", output));
		}
	}

	describe("create-fresh-queue") {
		before_each() {
			cd(getenv("HOME"));
			cd("..");
		}

		it("should create a fresh queue file") {
			FILE *listing;

			char *ls = calloc(LINESIZE, sizeof(*ls));
			check_alloc(ls);

			slinef(ls, "ls %s 2>/dev/null", qpath);

			listing = popen(ls, "r");
			if (fgets(output, LINESIZE, listing)) chomp(output);
			check(!(linecmp(output, qpath)));
			pclose(listing);

			system("./q create-fresh-queue");

			listing = popen(ls, "r");
			if (fgets(output, LINESIZE, listing)) chomp(output);
			check(linecmp(output, qpath));
			pclose(listing);

			cleanup_testq();
		}

		it("should error out if queue already exists") {
			FILE *redundancy;

			char *err = calloc(LINESIZE, sizeof(*err));
			check_alloc(err);

			slinef(err, "The file \"%s\" already exists.", qpath);

			system("./q create-fresh-queue 2>/dev/null");

			redundancy = popen("./q create-fresh-queue", "r");
			if (fgets(output, LINESIZE, redundancy)) chomp(output);
			check(linecmp(output, err));
			pclose(redundancy);

			cleanup_testq();
		}
	}

	describe("del") {
		context("with a populated queue") {
			before_each() {
				prep_testq();

				cd(getenv("HOME"));
				cd("..");
			}

			after_each() {
				cleanup_testq();
			}

			it("should delete the first item in the queue and keep the second") {
				/* Consider moving this test to feature spec. */
				system("./q del > /dev/null");

				char *numlisting = "   1 SECOND LINE\n";
				FILE *qlist = popen("./q list-all 2>&1", "r");

				check(filestringcmp(qlist, numlisting));
				pclose(qlist);
			}

			it("should show deleted item and new top item after deletion") {
				char *linedel = "deleted: FIRST LINE\n";
				char *newtop = "SECOND LINE\n";

				FILE *qdel = popen("./q del 2>&1", "r");

				fgets(output, LINESIZE, qdel);
				check(linecmp(output, linedel));

				fgets(output, LINESIZE, qdel);
				check(linecmp(output, newtop));
			}
		}

		it("should error out with no existing queue") {
			check(test_no_queue_error("./q del 2>&1", output));
		}
	}

	describe("list-all") {
		it("should error out with no existing queue") {
			check(test_no_queue_error("./q list-all 2>&1", output));
		}

		it("should error out with empty queue") {
			cd(getenv("HOME"));
			cd("..");

			char *err = (
				"Error reading from queuefile (it may be empty): "
				"Undefined error: 0"
			);

			system("./q create-fresh-queue 2>/dev/null");
			FILE *qlist = popen("./q list-all 2>&1", "r");

			if (fgets(output, LINESIZE, qlist)) chomp(output);
			check(linecmp(output, err));
			pclose(qlist);

			cleanup_testq();
		}

		context("with a populated queue") {
			before_each() {
				prep_testq();

				cd(getenv("HOME"));
				cd("..");
			}

			after_each() {
				cleanup_testq();
			}

			it("should print out a numbered list of queue entries") {
				char *numlisting = "   1 FIRST LINE\n   2 SECOND LINE\n";
				FILE *qlist = popen("./q list-all 2>&1", "r");

				check(filestringcmp(qlist, numlisting));
				pclose(qlist);
			}
		}
	}

	describe("show") {
		it("should print the first item in the queue") {
			prep_testq();

			cd(getenv("HOME"));
			cd("..");

			char *firstline = "FIRST LINE\n";
			fgets(output, LINESIZE, popen("./q 2>&1", "r"));

			check(linecmp(output, firstline));

			cleanup_testq();
		}

		it("should error out with empty queue") {
			cd(getenv("HOME"));
			cd("..");

			char *err = (
				"Error reading from queuefile (it may be empty): "
				"Undefined error: 0"
			);

			system("./q create-fresh-queue 2>/dev/null");
			FILE *qhead = popen("./q 2>&1", "r");

			if (fgets(output, LINESIZE, qhead)) chomp(output);
			check(linecmp(output, err));
			pclose(qhead);

			cleanup_testq();
		}
	}
}
