default:
	clang -o q src/q.c src/cli.c src/features.c src/utils.c

sync:
	fossil sync
	fossgit $(GITCLONES)/q/

testcli:
	clang src/cli_spec.c src/cli.c src/utils.c
	a.out
	rm a.out

testfeatures:
	clang src/features_spec.c src/features.c src/cli.c src/utils.c src/test_helpers.c
	HOME="`pwd`/tmp" a.out
	rm a.out

testutils:
	clang src/utils_spec.c src/utils.c src/cli.c src/test_helpers.c
	HOME="`pwd`/tmp" a.out
	rm a.out

test: testcli testfeatures testutils
	default
