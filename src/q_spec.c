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

	describe("list-all") {
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

		it("should print out a numbered list of queue entries") {
			prep_testq();

			cd(getenv("HOME"));
			cd("..");

			char *numlisting = "   1 FIRST LINE\n   2 SECOND LINE\n";
			FILE *qlist = popen("./q list-all 2>&1", "r");

			check(filestringcmp(qlist, numlisting));
			pclose(qlist);

			cleanup_testq();
		}

		context("without an existing queue") {
			before_each() {
				cd(getenv("HOME"));
				cd("..");
			}

			it("should error out with no existing queue") {
				char *err = "No queuefile found.  Try `./q create-fresh-queue`.";
				FILE *qlist = popen("./q list-all", "r");

				if (fgets(output, LINESIZE, qlist)) chomp(output);
				check(linecmp(output, err));
				pclose(qlist);

				cleanup_testq();
			}
		}
	}

	describe("show") {
		before_each() {
			prep_testq();

			cd(getenv("HOME"));
			cd("..");
		}

		after_each() {
			cleanup_testq();
		}

		it("should print the first item in the queue") {
			char *firstline = "FIRST LINE\n";
			fgets(output, LINESIZE, popen("./q 2>&1", "r"));

			check(linecmp(output, firstline));
		}
	}
}
