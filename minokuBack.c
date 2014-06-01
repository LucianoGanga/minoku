#include <stdio.h>
#include "getnum.h"
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <math.h>

#include "minokuBack.h"

void
generaTableros(char*** tablero, char*** tableroOculto, int fil, int col, int bombas) {
   sizeTablero(tablero, fil, col);
   sizeTablero(tableroOculto, fil, col);

   poneBombas(tableroOculto, fil - 1, col - 1, bombas);


   return;
}

void
sizeTablero(char*** matriz, int fil, int col) {

   int i, j;

   *matriz = realloc(*matriz, fil * sizeof (**matriz));

   for (i = 0; i < fil; i++) {
      (*matriz)[i] = realloc((*matriz)[i], col * sizeof (char));
   }

   for (i = 0; i < fil; i++) {
      for (j = 0; j < col; j++) {
         if (i == 0)
            (*matriz)[i][j] = j;
         else if (j == 0)
            (*matriz)[i][j] = i - 1 + 'A';
         else
            (*matriz)[i][j] = '.';
      }
   }


   return;
}

void
poneBombas(char ***matriz, int fil, int col, int bombas) {
   int vec[fil * col];
   int i, aux, r;

   for (i = 0; i < fil * col; i++)
      vec[i] = i;

   for (i = 0; i < bombas; i++) {
      r = rand() % (fil * col);
      aux = vec[i];
      vec[i] = vec[r];
      vec[r] = aux;
   }


   for (i = 0; i < bombas; i++) {
      (*matriz)[((vec[i] / fil) % fil) + 1][(vec[i] % col) + 1] = '#';

   }

   return;
}

int
fflag(char***tablero, int fil, int col, char x1, int y, int *mov, int *cantFlags, int *ultimaJugada) {
   int x = toupper(x1) - 'A' + 1;

   if (x < 1 || x > fil || y < 1 || y > col || *cantFlags == 0)
      return NO;

   if ((*tablero)[x][y] != '&') {
      (*tablero)[x][y] = '&';
      (*cantFlags)--;
      (*mov)--;
   }
   *ultimaJugada = FLAG;
   return SI;
}

int
unflag(char***tablero, int fil, int col, char x1, int y, int *mov, int *cantFlags, int *ultimaJugada) {
   int x = toupper(x1) - 'A' + 1;

   if (x < 1 || x > fil || y < 1 || y > col)
      return NO;

   if ((*tablero)[x][y] == '&') {
      (*tablero)[x][y] = '.';
      (*cantFlags)++;
      (*mov)--;
   }
   *ultimaJugada = UNFLAG;
   return SI;
}

void
seteaModo(TipoDificultad *modo, int fil, int col, int dificultad) {

   switch (dificultad) {
      case 1:
      {
         (modo->undos) = 10;
         (modo->bombas) = (fil * col)*20 / 100;
         (modo->cantFlags) = (fil * col)*20 / 100;
         break;
      }
      case 2:
      {
         (modo->undos) = 5;
         (modo->bombas) = (fil * col)*50 / 100;
         (modo->cantFlags) = (fil * col)*50 / 100;
         break;
      }
      case 3:
      {
         (modo->undos) = 3;
         (modo->bombas) = (fil * col)*70 / 100;
         (modo->cantFlags) = (fil * col)*70 / 100;
         break;
      }
      case 4:
      {
         (modo->undos) = 1;
         (modo->bombas) = (fil * col)*90 / 100;
         (modo->cantFlags) = (fil * col)*90 / 100;
         break;
      }
   }
   return;
}

void
cargaJuego(char * nombreArchivo) {

}

void
cierraJuego() {
   exit(0);
   return;
}

void
juegoCampania() {
   int fil, col;
   TipoDificultad modo;

   /*seteaModo(&modo, &fil, &col);*/

   /*jugar(modo);*/
}

int
flagline(char***tablero, int fil, int col, char x11, int y1, char x22, char y2, int *mov, int *cantFlags, int *ultimaJugada) {
   int x1 = toupper(x11) - 'A' + 1;
   int x2 = toupper(x22) - 'A' + 1;
   /*printf("%d\t%d\t%d\t%d\nflags:%d\n\n",x1,y1,x2,y2,*cantFlags);*/
   int i;
   printf("%d\n\n", *cantFlags);
   if (x1 < 1 || x1 > fil || y1 < 1 || y1 > col || x2 < 1 || x2 > fil || y2 < 1 || y2 > col || ((x1 != x2)&&(y1 != y2)))
      return NO;

   if (x1 == x2) {
      if (fabs(y1 - y2) + 1 > *cantFlags)
         return NO;
      for (i = 0; i <= fabs(y1 - y2); i++) {
         if ((*tablero)[x1][y1 < y2 ? y1 + i : y2 + i] != '&') {
            (*tablero)[x1][y1 < y2 ? y1 + i : y2 + i] = '&';
            (*cantFlags)--;
         }
      }

      (*mov)--;
   } else if (y1 == y2) {
      if (fabs(x1 - x2)>*cantFlags)
         return NO;
      for (i = 0; i <= fabs(x1 - x2); i++) {
         if ((*tablero)[x1 < x2 ? x1 + i : x2 + i][y1] != '&') {
            (*tablero)[x1 < x2 ? x1 + i : x2 + i][y1] = '&';
            (*cantFlags)--;
         }
      }

      (*mov)--;
   }

   printf("\n%d\n\n", *cantFlags);

   *ultimaJugada = FLAGLINE;
   return SI;
}

int
unflagline(char***tablero, int fil, int col, char x11, int y1, char x22, char y2, int *mov, int *cantFlags, int *ultimaJugada) {
   int x1 = toupper(x11) - 'A' + 1;
   int x2 = toupper(x22) - 'A' + 1;
   /*printf("%d\t%d\t%d\t%d\nflags:%d\n\n",x1,y1,x2,y2,*cantFlags);*/
   int i;
   printf("%d\n\n", *cantFlags);
   if (x1 < 1 || x1 > fil || y1 < 1 || y1 > col || x2 < 1 || x2 > fil || y2 < 1 || y2 > col || ((x1 != x2)&&(y1 != y2)))
      return NO;

   if (x1 == x2) {
      for (i = 0; i <= fabs(y1 - y2); i++) {
         if ((*tablero)[x1][y1 < y2 ? y1 + i : y2 + i] == '&') {
            (*tablero)[x1][y1 < y2 ? y1 + i : y2 + i] = '.';
            (*cantFlags)++;
         }
      }

      (*mov)--;
   } else if (y1 == y2) {
      for (i = 0; i <= fabs(x1 - x2); i++) {
         if ((*tablero)[x1 < x2 ? x1 + i : x2 + i][y1] == '&') {
            (*tablero)[x1 < x2 ? x1 + i : x2 + i][y1] = '.';
            (*cantFlags)++;
         }
      }

      (*mov)--;
   }

   printf("\n%d\n\n", *cantFlags);

   *ultimaJugada = UNFLAGLINE;
   return SI;
}

int
estadoDeJuego(char*** tablero, char*** tableroOculto, int fil, int col, TipoDificultad *modo) {
   int i, j, fin;

   /* Se fija si hay un flag en cada bomba. */
   for (fin = GANO, i = 0; i < fil + 1 && fin == GANO; i++) {
      for (j = 0; j < col + 1 && fin == GANO; j++) {
         if ((*tableroOculto)[i][j] == '#' && (*tablero)[i][j] != '&')
            fin = SIGUE;
      }
   }
   if (fin == GANO)
      return fin;


   /* Se fija si todos los casilleros sin bombas fueron barridos. */
   for (fin = GANO, i = 0; i < fil + 1 && fin == GANO; i++) {
      for (j = 0; j < col + 1 && fin == GANO; j++) {
         if ((*tableroOculto)[i][j] == '.' && (*tablero)[i][j] != '-')
            fin = SIGUE;
      }
   }
   if (fin == GANO)
      return fin;


   /* Se fija si se quedo sin movimientos. */
   if ((modo->mov) == 0)
      return PERDIO;

   for (i = 0; i < fil + 1; i++)
      for (j = 0; j < col + 1; j++)
         if ((*tablero)[i][j] == '#')
            return modo->undos > 0 && modo->mov > 1 ? HAYBOMBA : PERDIO;

   

   return SIGUE;

}