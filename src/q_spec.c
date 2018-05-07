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
		after_each() {
			cd("..");
		}

		it("should create a fresh queue file") {
			FILE *listing;

			char *ls = calloc(LINESIZE, sizeof(*ls));
			check_alloc(ls);

			snprintf(ls, LINESIZE, "ls %s 2>/dev/null", qpath);

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

			snprintf(err, LINESIZE, "The file \"%s\" already exists.", qpath);

			system("./q create-fresh-queue 2>/dev/null");

			redundancy = popen("./q create-fresh-queue", "r");
			if (fgets(output, LINESIZE, redundancy)) chomp(output);
			check(linecmp(output, err));
			pclose(redundancy);

			cleanup_testq();
		}
	}
}
