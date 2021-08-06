CCOMPILER?=clang -std=c99 -pedantic-errors
CC=$(CCOMPILER)

TESTCC?=MALLOC_OPTIONS=S $(CC)

default:
	$(CC) -o q src/q.c src/cli.c src/features.c src/utils.c

help:
	@echo 'USAGE: [CCOMPILER=<command>] make [target]'
	@echo ''
	@echo 'TARGETS:'
	@echo '	default       build q binary'
	@echo '	local         build q binary and move to ~/bin/q'
	@echo '	sync          sync Fossil repo and update Git mirror'
	@echo '	testcli       run CLI library tests'
	@echo '	testfeatures  run Features library tests'
	@echo '	testq         build and test a q binary'
	@echo '	testutils     run Utils library tests'
	@echo '	test          run CLI, Features, Utils and q build tests'

local: default
	mv q ~/bin/

sync:
	fossil sync
	fossgit

testcli:
	@$(TESTCC) src/cli_spec.c src/cli.c src/features.c src/utils.c
	@HOME="`pwd`/tmp" a.out
	@rm a.out

testfeatures:
	@$(TESTCC) src/features_spec.c src/features.c src/cli.c src/utils.c src/test_helpers.c
	@HOME="`pwd`/tmp" a.out
	@rm a.out

testq:
	@$(TESTCC) -o q src/q.c src/cli.c src/features.c src/utils.c -DDIRNAME='".quebert"'
	@$(TESTCC) src/q_spec.c src/test_helpers.c src/utils.c -DDIRNAME='".quebert"'
	@HOME="`pwd`/tmp" a.out
	@rm a.out
	@rm q

testutils:
	@$(TESTCC) src/utils_spec.c src/utils.c src/test_helpers.c
	@HOME="`pwd`/tmp" a.out
	@rm a.out

test: testcli testfeatures testq testutils
