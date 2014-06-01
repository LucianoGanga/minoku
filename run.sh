#!/bin/bash
clear
limArgumentos="1"
red='\033[0;31m'
yellow='\033[1;33m'
NC='\033[0m'
if [ $limArgumentos = "$#" ]
then
	echo -e "${yellow}-----> Generando ejecutable... \n${NC}"
	echo -e "${yellow}----- WARNINGS AND ERRORS: ------\n${NC}"
	#gcc -Wall -pedantic -g -o $1.out $1.c getnum2.c -lm minokuBack.c minokuFront.c minokuBack.h
	gcc -Wall -pedantic -g -o $1.out $1.c getnum2.c -lm
	if [ "$?" = 0 ]
	then
		sleep 1s
		clear
		./$1.out
	else
		echo -e "${red}\n^^^^^^ ERROR ^^^^^^\n${NC}"
	fi
else
	echo -e "${red}****** Cantidad de argumentos incorrecta. ******${NC}"
fi
