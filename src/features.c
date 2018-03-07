#include "features.h"

char *del_item() {
	return del_line(QNAME);
}

void del(char *self) {
	if (qexists()) {
		printf("%s", del_item());
	} else {
		print_error_qfile_missing(self);
	}
}
