default:
	clang -o q src/q.c src/help.c src/utils.c

sync:
	fossil sync
	fossgit $(GITCLONES)/q/

helptest:
	clang src/help_spec.c src/help.c
	a.out
	rm a.out

utilstest:
	clang src/utils_spec.c src/utils.c
	a.out
	rm a.out

test: helptest utilstest
