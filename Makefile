all:
	gcc -o minoku minokuFront.c minokuBack.c minokuBack.h getnum.h getnum.c -lm -g
debug: 
	gcc -Wall -pedantic -o minoku minokuFront.c minokuBack.c minokuBack.h getnum.h getnum.c -lm -g