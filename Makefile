default:
	clang -o q src/q.c src/cli.c src/utils.c

sync:
	fossil sync
	fossgit $(GITCLONES)/q/

clitest:
	clang src/cli_spec.c src/cli.c src/utils.c
	a.out
	rm a.out

utilstest:
	clang src/utils_spec.c src/utils.c
	HOME="`pwd`/tmp" a.out
	rm a.out

test: clitest utilstest
