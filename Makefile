default:
	clang -o q src/q.c src/cli.c src/features.c src/utils.c

help:
	@echo '	default       build q binary'
	@echo '	local         build q binary and move to ~/bin/q'
	@echo '	qtest         build q with .queuebert default dirname'
	@echo '	sync          sync Fossil repo and update Git mirror'
	@echo '	testcli       run CLI library tests'
	@echo '	testfeatures  run Features library tests'
	@echo '	testutils     run Utils library tests'
	@echo '	test          run CLI, Features, Utils and q build tests'

local: default
	mv q ~/bin/

qtest:
	clang -o q src/q.c src/cli.c src/features.c src/utils.c -DDIRNAME='".quebert"'

sync:
	fossil sync
	fossgit $(GITCLONES)/q/

testcli:
	@: clang src/cli_spec.c src/cli.c src/features.c src/utils.c
	@: HOME="`pwd`/tmp" a.out
	@: rm a.out

testfeatures:
	@: clang src/features_spec.c src/features.c src/cli.c src/utils.c src/test_helpers.c
	@: HOME="`pwd`/tmp" a.out
	@: rm a.out

testutils:
	@: clang src/utils_spec.c src/utils.c src/test_helpers.c
	@: HOME="`pwd`/tmp" a.out
	@: rm a.out

test: testcli testfeatures testutils qtest
