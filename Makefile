all:
	gcc minokuFront.c minokuBack.c minokuBack.h getnum.h getnum.c -o minoku  -lm -g
debug:
	gcc -pedantic -Wall minokuFront.c minokuBack.c minokuBack.h getnum.h getnum.c -o minoku  -lm -g
