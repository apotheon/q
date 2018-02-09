all:
	clang -o q src/q.c src/help.c

helptest:
	clang src/help_spec.c src/help.c
	a.out

test: helptest
