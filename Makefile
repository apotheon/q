default:
	clang -o q src/q.c src/help.c

sync:
	fossil sync
	fossgit $(GITCLONES)/q/

helptest:
	clang src/help_spec.c src/help.c
	a.out

test: helptest
