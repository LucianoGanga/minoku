#include <stdio.h>
#include "getnum.h"
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include "minokuBack.h"

void initMinoku();
void menu();
void submenu(int);
void juegoIndividual(int);
void pideDimensiones(int *, int *);
void imprimeTablero(char*** , int, int);
int jugar(char***, char***, int, int, TipoDificultad *);



int
main() {
	initMinoku();
	return 0;
}

void
initMinoku() {
    /* Seteo un seed random */
    srand(time(NULL));

	limpiaPantalla();
    printf("Bienvenido a MINOKU\n\n");
    menu();
}


void menu() {
    int opcion = -1;

    printf("Ingrese una opcion para comenzar: \n");
    printf("1) Juego nuevo.\n");
    printf("2) Recuperar juego grabado.\n");
    printf("3) Terminar.\n");

    opcion = getint("");

    /* Si el usuario se equivoca, le muestro un mensaje para que lo reintente */
    if (opcion < 1 || opcion > 3) {
        printf("La opcion ingresada no es valida. \n\n");
        menu();
    } else {
        submenu(opcion);
    }

}

void submenu(int nroMenu) 
{
    int opcion;

   	limpiaPantalla();
    switch (nroMenu) {
        case 1:
            printf("Ingrese el modo de juego: \n");
            printf("1) Juego individual sin limite de movimientos.\n");
            printf("2) Juego individual con limite de movimientos.\n");
            printf("3) Juego por campaña.\n");

            opcion = getint("");

            /* Si el usuario se equivoca, le muestro un mensaje para que lo reintente */
            if (opcion < 1 || opcion > 3) {
                printf("La opcion ingresada no es valida. \n\n");
                submenu(nroMenu);
            } else {
                if (opcion == 1 || opcion == 2)
                    juegoIndividual(opcion);
                if (opcion == 3)
                    juegoCampania();
            }

            break;
        case 2:
                {
            char c;
            char * nombreArchivo;
            printf("Escriba el nombre de la partida que desea cargar: \n");
            /* @TODO: Cambiar esto por las líneas que permiten leer el string y ponerlo en una variable */
            while ((c = getchar()) != EOF)
                putchar(c);

            cargaJuego(nombreArchivo);
        }
 		break;
        case 3:
        {
            /* @TODO: Cambiar esto por una función que de opción "S" y "N" para salir del juego o volver al menu principal */
            int salir = -1;
            salir = getint("Esta seguro que desea salir?: \n");
            
            if(salir == 1)
	            cierraJuego();
	        else 
	        	limpiaPantalla();
	            menu();
        }
            break;

    }
}

void
juegoIndividual(int opcion) 
{
	int fil, col, dificultad,fin;
	TipoDificultad modo;
	pideDimensiones(&fil, &col);
	dificultad=pideNivelDificultad(fil, col);
	char **tablero=NULL, **tableroOculto=NULL;

		if(opcion == 1)
		{	printf("Eligio juego individual sin limite de movimientos");
			modo.mov=-1;
			seteaModo(&modo, fil, col, dificultad); 
		}
		else
		{	printf("Eligio juego individual con limite de movimientos");
			seteaModo(&modo, fil, col, dificultad); 
			modo.mov=modo.bombas+modo.undos;
		}
		   
		//printf("fil %d col %d\n", fil, col);

		generaTableros(&tablero, &tableroOculto, fil+1, col+1,modo.bombas);

		//imprimeTablero(&tablero, fil, col);
		printf("\n\n");
		//imprimeTablero(&tableroOculto, fil, col);
		
		fin=jugar(&tablero,&tableroOculto,fil,col,&modo);

}


void pideDimensiones(int * fil, int * col) 
{
    while (*fil < MINFIL || *fil > MAXFIL) {
        *fil = getint("¿Cuantas filas tendra el tablero?");
        if (*fil < MINFIL || *fil > MAXFIL)
            printf("La cantidad de filas debe ser entre 5 y 19.\n");
    }
    while (*col < MINCOL || *col > MAXCOL) {
        *col = getint("¿Cuantas columnas tendra el tablero?");
        if (*col < MINCOL || *col > MAXCOL)
            printf("La cantidad de columnas debe ser entre 5 y 19.\n");
    }
    return;
}

int pideNivelDificultad(int fil, int col) 
{
    /* Nivel de dificultad - Pide filas y columnas para saber si se ingreso un nivel correcto */
    int nivelDificultad;

    printf("Escoja la dificultad:\n");
    printf("1) Facil\n");
    printf("2) Medio\n");
    printf("3) Dificil\n");
    printf("4) Pesadilla\n\n");
    /* Pido al usuario que ingrese la opción */
    nivelDificultad = getint("");

    
    if (nivelDificultad < 1 || nivelDificultad > 4) {
        printf("La opcion ingresada no es valida. \n\n");
        nivelDificultad = pideNivelDificultad(fil, col);
    }

    if (nivelDificultad == 4 && (fil)*(col) < 100) {
        printf("Este modo de juego es solo para tableros de mas de 100 casilleros.\n");
        nivelDificultad = pideNivelDificultad(fil, col);
    }
    
    return nivelDificultad;
}

void
limpiaPantalla() 
{
	printf("\033[2J\033[1;1H");
	return;
}


void
imprimeTablero(char*** matriz, int fil, int col) 
{
    int i, j;

    for (i = 0; i < fil+1; i++) {
        for (j = 0; j < col+1; j++) {

/* @TODO: Reemplazar esto por una macro que me imprima el caracter que yo quiera. La matríz debería devolverme sólo el tipo de casillero, y desde el frontEnd debería interpretarlo y mostrar una X, una O o lo que yo quiera dependiendo el caso. */
            if (i == 0)
                printf("%d\t", (*matriz)[i][j]); 
            else
                printf("%c\t", (*matriz)[i][j]);
        }
        printf("\n");
    }
    return;
}

int
jugar(char*** tablero, char*** tableroOculto, int fil, int col, TipoDificultad *modo)
{
	int ultimaJugada=UNDO, fin=SIGUE, pudoEscanear;
	TipoCoordenada coord;

	while(fin==SIGUE)
	{	//limpiaPantalla();
		imprimeTablero(tablero, fil, col);
		printf("\n");
		imprimeTablero(tableroOculto, fil, col);
		pudoEscanear=escanear(tablero,tableroOculto,fil,col,modo,&ultimaJugada,&coord);
		//printf("%d\n",pudoEscanear);
		//printf("\n\n");
		//printf("\n%d\n\n", (modo->cantFlags));
	}

	return 0;

}

int
queryCol(char*** tableroOculto, int fil, int col, int columna)
{
	int i, b=0;
	printf("\n\n");

	if(columna<1 || columna>col)
		return NO;

	for(i=1;i<fil+1;i++)
	{	if((*tableroOculto)[i][columna]=='#')
			b++;
		else
			if(b!=0)
			{	printf("%d\t",b);
				b=0;
			}
	}
	if(b!=0)
		printf("%d\n",b);
	else
		printf("\n");

	return SI;
}
		
int
queryFil(char*** tableroOculto, int fil, int col, char fila)
{	
	int i, b=0;
	printf("\n\n");

	if((toupper(fila)-'A'+1)<1 || (toupper(fila)-'A'+1)>fil)
		return NO;
	
	for(i=1;i<col+1;i++)
	{	if((*tableroOculto)[toupper(fila)-'A'+1][i]=='#')
		{	
			b++;
		}
		else
			if(b!=0)
			{	printf("%d\t",b);
				b=0;
			}
	}
	if(b!=0)
		printf("%d\n",b);
	else
		printf("\n");

	return SI;
}

int
escanear(char*** tablero, char*** tableroOculto, int fil, int col, TipoDificultad *modo, int *ultimaJugada, TipoCoordenada *coord)
{	
	char *string=NULL, *aux=NULL;
	int b,c,d;
	char *biblioStrings[CANTSTRINGS]={"S","query","flag","unflag","save","quit","undo"};
	int i=0,j=0,flag=0,parametros,tipoDeQuery;
	
	printf("Ingrese comando:\n");
		
	while(!isspace(c=getchar()))
	{
		if(i%BLOQUE==0)
		{
			aux=realloc(string,(i+BLOQUE)*sizeof(*string));
			if(aux==NULL)
			{	free(string);
				printf("No hay memoria.\n"); 
				return 0;
			}
			string=aux;
		}
		string[i]=c;
		i++;
	}

	string=realloc(string,i*sizeof(*string));
	string[i]=0;

	for(i=0;i<CANTSTRINGS && flag==0;i++)
	{
		if(strcmp(string,biblioStrings[i])==0)
		{	//printf("Usted puso %s\n",biblioStrings[i]);
			flag=1;
		}
	}

	i--;

	if(flag==0)
	{	printf("No se reconoce el comando\n");
		return NO;
	}
	
	switch(i)
	{	case 2:
		case 3:
		{
			parametros=scanf("(%c,%d,%c,%d", &(coord->x1),&(coord->y1),&(coord->x2),&(coord->y2));
			c=getchar();
			if(c!='\n')
				d=getchar();
			if((parametros==2) && c==')' && d=='\n')
			{	if(i==2)
				{	
					if(fflag(tablero,fil,col,coord->x1,coord->y1,&(modo->mov),&(modo->cantFlags),ultimaJugada)==NO)
						printf("Parametros inexistentes\n");
				}
				else
				{
					if(unflag(tablero,fil,col,coord->x1,coord->y1,&(modo->mov),&(modo->cantFlags),ultimaJugada)==NO)
						printf("Parametros inexistentes\n");
				}
			}
			else if((parametros==4) && c==')' && d=='\n')
			{	if(i==2)
				{	if(flagline(tablero,fil,col,coord->x1,coord->y1,coord->x2,coord->y2,&(modo->mov),&(modo->cantFlags),ultimaJugada)==NO)
						printf("Parametros inexistentes\n");
				}
				else
				{if(unflagline(tablero,fil,col,coord->x1,coord->y1,coord->x2,coord->y2,&(modo->mov),&(modo->cantFlags),ultimaJugada)==NO)
						printf("Parametros inexistentes\n");
				}
			}			
			else
			{	printf("No se reconoce el comando\n");
				return NO;
			}
			break;
		}
		case 0:
		{	parametros=scanf("(%c,%d", &(coord->x1),&(coord->y1));
			c=getchar();
			if(c!='\n')
				d=getchar();
			if((parametros==2) && c==')' && d=='\n')
				printf("%s\n",biblioStrings[i]);
			else
			{	printf("No se reconoce el comando\n");
				return NO;
			}
			break;
		}
		case 1:
		{	parametros=scanf("%d",&(coord->y1));
			if(parametros!=1)
			{	c=getchar();
				tipoDeQuery=QUERYFILA;
			}
			else
				tipoDeQuery=QUERYCOL;
			if(tipoDeQuery==QUERYFILA && isalpha(c))
			{	d=getchar();
					if(d=='\n')
					{	if(queryFil(tableroOculto,fil,col,c)==NO)
							printf("La fila no existe en el tablero.\n");
					}
				else
				{	printf("No se reconoce el comando\n");
					return NO;
				}
			}
			else if(tipoDeQuery==QUERYCOL && (d=getchar())=='\n') 
				{
				if(queryCol(tableroOculto,fil,col,coord->y1)==NO)
					printf("La columna no existe en el tablero.\n");
				}
			else
			{	printf("No se reconoce el comando\n");
				return NO;
			}
			break;
		}
		case 4:
		{	printf("%s\n",biblioStrings[i]);
			break;
		}
		case 5:
		{	printf("%s\n",biblioStrings[i]);
			break;
		}
		case 6:
		{	printf("%s\n",biblioStrings[i]);
			break;
		}
	}


	
	return SI;
}
