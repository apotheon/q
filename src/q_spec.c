#include "utils.h"
#include "test_helpers.h"
#include "features.h"
#include "bdd-for-c.h"

spec("queuer") {
	char *qpath = calloc(LINESIZE, sizeof(*qpath));
	check_alloc(qpath);
	snprintf(qpath, LINESIZE, "tmp/%s/%s", DIRNAME, QNAME);

	char *ls = calloc(LINESIZE, sizeof(*ls));
	check_alloc(ls);

	describe("create-fresh-queue") {
		it("should create a fresh queue file") {
			uint32_t count;
			FILE *listing;

			char *output = calloc(LINESIZE, sizeof(*output));
			check_alloc(output);

			snprintf(ls, LINESIZE, "ls %s 2>/dev/null", qpath);

			listing = popen(ls, "r");
			if (fgets(output, LINESIZE, listing)) chomp(output);
			check(!(linecmp(output, qpath)));
			pclose(listing);

			system("./q create-fresh-queue");

			listing = popen(ls, "r");
			if (fgets(output, LINESIZE, listing)) {
				for (count = 0; *(output + count); ++count);
				*(output + count - 1) = '\0';
			}
			check(linecmp(output, qpath));
			pclose(listing);

			cleanup_testq();
		}
	}
}
