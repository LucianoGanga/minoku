#include<stdio.h>
#include"getnum.h"
#include<stdlib.h>
#include<time.h>
#include"minokuBack.h"



void
generaTableros(char*** tablero, char*** tableroOculto, int fil, int col, int bombas)
{
	sizeTablero(tablero,fil,col);
	sizeTablero(tableroOculto,fil,col);

	poneBombas(tableroOculto,fil-1,col-1,bombas);


	return;
}


void
sizeTablero(char*** matriz, int fil, int col)
{
	
	int i,j;

	*matriz=realloc(*matriz,fil*sizeof(**matriz));

	for(i=0;i<fil;i++)
	{
		(*matriz)[i]=realloc((*matriz)[i],col*sizeof(char));
	}

	for(i=0;i<fil;i++)
	{	for(j=0;j<col;j++)
		{
			if(i==0)
				(*matriz)[i][j]=j;
			else if(j==0)
				(*matriz)[i][j]=i-1+'A';
			else
				(*matriz)[i][j]='.';
		}
	}

	
	return;
}


void
poneBombas(char ***matriz, int fil, int col, int bombas)
{
	int vec[fil*col];
	int i, aux, r;

	for(i=0;i<fil*col;i++)
		vec[i]=i;

	for(i=0;i<bombas;i++)
	{	r=rand()%(fil*col);
		aux=vec[i];
		vec[i]=vec[r];
		vec[r]=aux;
	}


	for(i=0;i<bombas;i++)
	{
		(*matriz)[((vec[i]/fil)%fil)+1][(vec[i]%col)+1]='#';
		
	}

	return;	
}


















