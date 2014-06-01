all:
	gcc -o minoku minokuFront.c minokuBack.c minokuBack.h getnum.h getnum.c -lm -g
