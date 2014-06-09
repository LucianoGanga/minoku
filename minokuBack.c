#include <stdio.h>
#include "getnum.h"
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <math.h>

#include "minokuBack.h"

void
generaTableros(char*** tablero, char*** tableroOculto, int fil, int col, int bombas) {

   printf("Bombas: %d\n", bombas);

   sizeTablero(tablero, fil, col, TIPO_TABLERO_VISIBLE);
   sizeTablero(tableroOculto, fil, col, TIPO_TABLERO_OCULTO);

   poneBombas(tableroOculto, fil, col, bombas);


   return;
}

void
sizeTablero(char*** matriz, int fil, int col, int tipoTablero) {

   int i, j;

   *matriz = realloc(*matriz, fil * sizeof (**matriz));

   for (i = 0; i < fil; i++) {
      (*matriz)[i] = realloc((*matriz)[i], col * sizeof (char));
   }

   for (i = 0; i < fil; i++) {
      for (j = 0; j < col; j++) {
         (*matriz)[i][j] = (tipoTablero == TIPO_TABLERO_OCULTO) ? '-' : '0';
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
      (*matriz)[((vec[i] / fil) % fil)][(vec[i] % col)] = '#';
   }

   return;
}

int
fflag(char***tablero, int fil, int col, char x1, int y, int *mov, int *cantFlags, int *ultimaJugada) {
   int x = toupper(x1) - 'A';
   /* Resto 1 a la coordenada elegida para que coincida con la posición a la que se refiere el jugador */
   y = y - 1;

   if (x < 0 || x > fil || y < 0 || y > col)
      return NO;
   if (*cantFlags == 0)
      return NOFLAG;

   if ((*tablero)[x][y] != '&') {
      (*tablero)[x][y] = '&';
      (*cantFlags)--;
      (*mov)--;
      *ultimaJugada = FLAG;
   } else
      *ultimaJugada = NADA;

   return SI;
}

int
unflag(char***tablero, int fil, int col, char x1, int y, int *mov, int *cantFlags, int *ultimaJugada) {
   int x = toupper(x1) - 'A';
   /* Resto 1 a la coordenada elegida para que coincida con la posición a la que se refiere el jugador */
   y = y - 1;

   if (x < 0 || x > fil || y < 0 || y > col)
      return NO;

   if ((*tablero)[x][y] == '&') {
      (*tablero)[x][y] = '0';
      (*cantFlags)++;
      (*mov)--;
      *ultimaJugada = UNFLAG;

   } else
      *ultimaJugada = NADA;


   return SI;
}

void
seteaModo(TipoDificultad *modo, int fil, int col) {

   printf("NIVEL: %d\n", modo->nivelDificultad);
   switch (modo->nivelDificultad) {
      case 1:
      {
         (modo->undos) = 10;
         (modo->bombas) = (fil * col)*20 / 100;
         (modo->cantFlags) = (modo->bombas);
         break;
      }
      case 2:
      {
         (modo->undos) = 5;
         (modo->bombas) = (fil * col)*50 / 100;
         (modo->cantFlags) = (modo->bombas);
         break;
      }
      case 3:
      {
         (modo->undos) = 3;
         (modo->bombas) = (fil * col)*70 / 100;
         (modo->cantFlags) = (modo->bombas);
         break;
      }
      case 4:
      {
         (modo->undos) = 1;
         (modo->bombas) = (fil * col)*90 / 100;
         (modo->cantFlags) = (modo->bombas);
         break;
      }
   }
   return;
}

int
cargaJuego(char * nombreArchivo, char *** tablero, char*** tableroOculto, TipoDificultad * modo, int * fil, int * col) {
   FILE * archivo;
   int estadoLectura, i, j, flag = 0, movimientos;

   if (DEBUG_MODE)
      printf("Cargando el archivo %s\n", nombreArchivo);

   archivo = fopen(nombreArchivo, "rb");
   if (archivo == NULL)
      return ERROR_APERTURA_ARCHIVO;

   /* Cargo nivel de dificultad */
   if (DEBUG_MODE)
      printf("Carga nivel de dificultad...\n");

   estadoLectura = fread(&(modo->nivelDificultad), sizeof (modo->nivelDificultad), 1, archivo);
   if (estadoLectura != 1)
      return ERROR_LECTURA_ARCHIVO;


   /* Cargo filas */
   if (DEBUG_MODE)
      printf("Carga filas...\n");

   estadoLectura = fread(fil, sizeof (*fil), 1, archivo);
   if (estadoLectura != 1 || *fil < MINFIL || *fil > MAXFIL)
      return ERROR_LECTURA_ARCHIVO;

   /* Cargo filas */
   if (DEBUG_MODE)
      printf("Carga columnas...\n");

   estadoLectura = fread(col, sizeof (*col), 1, archivo);
   if (estadoLectura != 1 || *col < MINCOL || *col > MAXCOL)
      return ERROR_LECTURA_ARCHIVO;

   /* Seteo Modo */
   if (DEBUG_MODE)
      printf("Setea modo...\n");

   seteaModo(modo, *fil, *col);

   /* Cargo undos */
   if (DEBUG_MODE)
      printf("Carga undos...\n");

   estadoLectura = fread(&(modo->undos), sizeof (modo->undos), 1, archivo);
   if (estadoLectura != 1 || modo->undos < 0)
      return ERROR_LECTURA_ARCHIVO;

   /* Cargo movimientos */
   if (DEBUG_MODE)
      printf("Carga movimientos...\n");

   estadoLectura = fread(&movimientos, sizeof (movimientos), 1, archivo);
   if (estadoLectura != 1)
      return ERROR_LECTURA_ARCHIVO;

   modo->mov = (movimientos == 0) ? -1 : movimientos;


   /* Cargo modo de juego campania o individual */
   if (DEBUG_MODE)
      printf("Carga modo de juego...\n");

   estadoLectura = fread(&(modo->campania), sizeof (modo->campania), 1, archivo);
   if (estadoLectura != 1)
      return ERROR_LECTURA_ARCHIVO;

   /* Genero tableros */
   if (DEBUG_MODE)
      printf("Genera tableros...\n");

   generaTableros(tablero, tableroOculto, *fil, *col, modo->bombas);


   /* Cargo tablero oculto */
   if (DEBUG_MODE)
      printf("Carga matriz oculta...\n");

   for (i = 0; i < *fil; i++) {
      estadoLectura = fread((*tableroOculto)[i], sizeof (***tableroOculto), *col, archivo);
      if (estadoLectura != *col)
         return ERROR_LECTURA_ARCHIVO;
   }

   /* Cargo tablero visible */
   if (DEBUG_MODE)
      printf("Carga matriz visible...\n");

   for (i = 0; i < *fil; i++) {
      estadoLectura = fread((*tablero)[i], sizeof (***tablero), *col, archivo);

      /* Cargo cantidad de flags de acuerdo a los banderines de la matriz visible */
      for (j = 0; j<*col; j++)
         if ((*tablero)[i][j] == '&')
            flag++;
      modo->cantFlags = modo->bombas - flag;
      if (estadoLectura != *col)
         return ERROR_LECTURA_ARCHIVO;
   }

   fclose(archivo);

   /* En base a los parámetros que cargue, devuelvo un valor indicando el tipo de partida */


   /* Juego campania */
   if (modo->campania == 1)
      return 3;

   /* Juego sin limite de movimientos */
   if (modo->mov < 0)
      return 1;

   /* Juego con limite de movimientos */
   if (modo->mov > 0)
      return 2;

}

int
guardaJuego(char * nombreArchivo, char*** tablero, char*** tableroOculto, TipoDificultad * modo, int fil, int col) {
   int i, movimientos;
   FILE * archivo;
   if (DEBUG_MODE)
      printf("Guardando el archivo %s\n", nombreArchivo);

   archivo = fopen(nombreArchivo, "wb");
   if (archivo == NULL)
      return ERROR_APERTURA_ARCHIVO;

   /* Guardo nivel de dificultad */
   fwrite(&(modo->nivelDificultad), sizeof (modo->nivelDificultad), 1, archivo);
   /* Guardo filas */
   fwrite(&fil, sizeof (fil), 1, archivo);
   /* Guardo columnas */
   fwrite(&col, sizeof (col), 1, archivo);
   /* Guardo undos */
   fwrite(&(modo->undos), sizeof (modo->undos), 1, archivo);
   /* Guardo movimientos */
   movimientos = (modo->mov < 0) ? 0 : modo->mov;
   fwrite(&movimientos, sizeof (movimientos), 1, archivo);

   /* Guardo campania */
   fwrite(&(modo->campania), sizeof (modo->campania), 1, archivo);

   /* Guardo tablero oculto */
   if (DEBUG_MODE)
      printf("Guardando el tablero oculto del archivo %s\n", nombreArchivo);
   for (i = 0; i < fil; i++) {
      fwrite((*tableroOculto)[i], sizeof (***tableroOculto), col, archivo);
   }

   if (DEBUG_MODE)
      printf("Guardando el tablero visible del archivo %s\n", nombreArchivo);
   /* Guardo tablero visible */
   for (i = 0; i < fil; i++) {
      fwrite((*tablero)[i], sizeof (***tablero), col, archivo);
   }

   /* Cierro la escritura del archivo */
   fclose(archivo);
   return SI;
}

void
cierraJuego() {
   exit(0);
   return;
}

int
flagline(char***tablero, int fil, int col, char x11, int y1, char x22, char y2, int *mov, int *cantFlags, int *ultimaJugada) {
   int x1 = toupper(x11) - 'A';
   int x2 = toupper(x22) - 'A';
   /*printf("%d\t%d\t%d\t%d\nflags:%d\n\n",x1,y1,x2,y2,*cantFlags);*/
   int i;
   y1--;
   y2--;
   /*printf("%d\n\n", *cantFlags); */
   if (x1 < 0 || x1 > fil || y1 < 0 || y1 > col || x2 < 0 || x2 > fil || y2 < 0 || y2 > col || ((x1 != x2)&&(y1 != y2)))
      return NO;

   if (x1 == x2) {
      if (fabs(y1 - y2) + 1 > *cantFlags)
         return NOFLAG;
      for (i = 0; i <= fabs(y1 - y2); i++) {
         if ((*tablero)[x1][y1 < y2 ? y1 + i : y2 + i] != '&') {
            (*tablero)[x1][y1 < y2 ? y1 + i : y2 + i] = '&';
            (*cantFlags)--;
            (*mov)--;
            *ultimaJugada = FLAGLINE;
         }
      }


   } else if (y1 == y2) {
      if (fabs(x1 - x2)>*cantFlags)
         return NOFLAG;
      for (i = 0; i <= fabs(x1 - x2); i++) {
         if ((*tablero)[x1 < x2 ? x1 + i : x2 + i][y1] != '&') {
            (*tablero)[x1 < x2 ? x1 + i : x2 + i][y1] = '&';
            (*cantFlags)--;
            (*mov)--;
            *ultimaJugada = FLAGLINE;
         }
      }


   }

   /*printf("\n%d\n\n", *cantFlags); */


   return SI;
}

int
unflagline(char***tablero, int fil, int col, char x11, int y1, char x22, char y2, int *mov, int *cantFlags, int *ultimaJugada) {
   int x1 = toupper(x11) - 'A';
   int x2 = toupper(x22) - 'A';
   /*printf("%d\t%d\t%d\t%d\nflags:%d\n\n",x1,y1,x2,y2,*cantFlags);*/
   int i;

   y1--;
   y2--;

   /* printf("%d\n\n", *cantFlags); */
   if (x1 < 0 || x1 > fil || y1 < 0 || y1 > col || x2 < 0 || x2 > fil || y2 < 0 || y2 > col || ((x1 != x2)&&(y1 != y2)))
      return NO;

   if (x1 == x2) {
      for (i = 0; i <= fabs(y1 - y2); i++) {
         if ((*tablero)[x1][y1 < y2 ? y1 + i : y2 + i] == '&') {
            (*tablero)[x1][y1 < y2 ? y1 + i : y2 + i] = '0';
            (*cantFlags)++;
            *ultimaJugada = UNFLAGLINE;
            (*mov)--;
         }
      }


   } else if (y1 == y2) {
      for (i = 0; i <= fabs(x1 - x2); i++) {
         if ((*tablero)[x1 < x2 ? x1 + i : x2 + i][y1] == '&') {
            (*tablero)[x1 < x2 ? x1 + i : x2 + i][y1] = '0';
            (*cantFlags)++;
            *ultimaJugada = UNFLAGLINE;
            (*mov)--;
         }
      }
   }

   /*printf("\n%d\n\n", *cantFlags); */


   return SI;
}

int
barrer(char***tablero, char***tableroOculto, int fil, int col, char x1, int y1, int *mov, int *ultimaJugada) {
   int x = toupper(x1) - 'A';
   y1--;
   if (x < 0 || x > fil || y1 < 0 || y1 > col) {
      return NO;
   }

   if ((*tableroOculto)[x][y1] == '#')
      (*tablero)[x][y1] = '#';
   else
      (*tablero)[x][y1] = '-';
   (*mov)--;
   *ultimaJugada = BARRER;


   return SI;

}

int
estadoDeJuego(char*** tablero, char*** tableroOculto, int fil, int col, TipoDificultad *modo) {
   int i, j, fin;

   /* Se fija si hay un flag en cada bomba. */
   for (fin = GANO, i = 0; i < fil && fin == GANO; i++) {
      for (j = 0; j < col && fin == GANO; j++) {
         if ((*tableroOculto)[i][j] == '#' && (*tablero)[i][j] != '&')
            fin = SIGUE;
      }
   }
   if (fin == GANO)
      return fin;


   /* Se fija si todos los casilleros sin bombas fueron barridos. */
   for (fin = GANO, i = 0; i < fil && fin == GANO; i++) {
      for (j = 0; j < col && fin == GANO; j++) {
         if (((*tableroOculto)[i][j] == '0' || (*tableroOculto)[i][j] == '-') && (*tablero)[i][j] != '-')
            fin = SIGUE;
      }
   }
   if (fin == GANO)
      return fin;


   /* Se fija si se quedo sin movimientos. */
   if ((modo->mov) == 0)
      return PERDIO;

   for (i = 0; i < fil; i++)
      for (j = 0; j < col; j++)
         if ((*tablero)[i][j] == '#')
            return modo->undos > 0 && modo->mov != 1 ? HAYBOMBA : PERDIO;

   return SIGUE;

}

int
undo(char***tablero, int fil, int col, TipoCoordenada *coord, int *mov, int *cantFlags, int*undos, int *ultimaJugada) {
   if (*ultimaJugada != NADA)
      switch (*ultimaJugada) {
         case FLAG:
         {
            unflag(tablero, fil, col, coord->x1, coord->y1, mov, cantFlags, ultimaJugada);
            /*printf("hola\n"); */
            break;
         }
         case UNFLAG:
         {
            fflag(tablero, fil, col, coord->x1, coord->y1, mov, cantFlags, ultimaJugada);
            break;
         }
         case FLAGLINE:
         {
            unflag(tablero, fil, col, coord->x2, coord->y2, mov, cantFlags, ultimaJugada);
            break;
         }
         case UNFLAGLINE:
         {
            fflag(tablero, fil, col, coord->x2, coord->y2, mov, cantFlags, ultimaJugada);
            break;
         }
         case BARRER:
         {
            int x = toupper(coord->x1) - 'A';
            (*tablero)[x][coord->y1 - 1] = '0';
            break;
         }
         case UNDO:
         {
            return NO;
            break;
         }
      }

   *ultimaJugada = UNDO;
   (*undos)--;
   return SI;
}