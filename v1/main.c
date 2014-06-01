#include <stdio.h>
#include "getnum.h"
#include <stdlib.h>
#include <time.h>
#include "minokuBack.h"


/* Setea la cantidad de bombas, undos, banderines, y la dimensiÃ³n que tendrÃ¡ el tablero */
void pideParametros(TipoDificultad *modo, int *fil, int *col);

void imprimeTablero(char***matriz, int fil, int col);

//void generaTableros(char*** tablero, char*** tableroOculto, int fil, int col, int bombas);
//void sizeTablero(char*** matriz, int fil, int col);
//void poneBombas(char ***matriz, int fil, int col, int bombas);

void initMinoku();
void menu();

void juegoNuevo();
void cargaJuego();
void cierraJuego();

int main() {
   // Seteo un seed random
   srand(time(NULL));

   // Defino variables
   int fil = 0, col = 0, dificultad, bombas;
   char **tablero = NULL, **tableroOculto = NULL;
   TipoDificultad modo;

   printf("Bienvenido a MINOKU\n\n");

   // Llamo al menu
   menu();
}

void
menu() {
   int opcion1 = 0;
   while (opcion1 < 0 || opcion1 > 3)
      opcion1 = getint("1) Juego nuevo.\n2) Recuperar juego grabado.\n3) Terminar.\n\n");

   

}

void
juegoNuevo() {
   int opcion2;
   switch (opcion2 = getint("1) Juego individual sin limite de movimientos.\n2) Juego individual con limite de movimientos.\n3) Juego por campaÃ±a.\n\n")) {
      case 1:
      {
         break;
      }
      case 2:
      {
         break;
      }
      case 3:
      {
      }
   }
}

void cierraJuego() {
   printf("Adios.\n");
   return;
}

void
pideParametros(TipoDificultad *modo, int *fil, int *col) {
   int dificultad = 0;
   *fil = *col = 0;

   while (*fil < MINFIL || *fil > MAXFIL) {
      *fil = getint("Â¿Cuantas filas tendra el tablero?");
      if (*fil < MINFIL || *fil > MAXFIL)
         printf("La cantidad de filas debe ser entre 5 y 19.\n");
   }
   while (*col < MINCOL || *col > MAXCOL) {
      *col = getint("Â¿Cuantas columnas tendra el tablero?");
      if (*col < MINCOL || *col > MAXCOL)
         printf("La cantidad de columnas debe ser entre 5 y 19.\n");
   }

   while ((dificultad < 1) || (dificultad > 4)) {
      switch (dificultad = getint("Escoja la dificultad:\n1) Facil\n2) Medio\n3) Dificil\n4) Pesadilla\n\n")) {
         case 1:
         {
            (modo->undos) = 10;
            (modo->bombas) = ((*fil)*(*col))*20 / 100;
            (modo->cantFlags) = ((*fil)*(*col))*20 / 100;
            break;
         }
         case 2:
         {
            (modo->undos) = 5;
            (modo->bombas) = ((*fil)*(*col))*50 / 100;
            (modo->cantFlags) = ((*fil)*(*col))*50 / 100;
            break;
         }
         case 3:
         {
            (modo->undos) = 3;
            (modo->bombas) = ((*fil)*(*col))*70 / 100;
            (modo->cantFlags) = ((*fil)*(*col))*70 / 100;
            break;
         }
         case 4:
         {
            if ((*fil)*(*col) >= 100) {
               (modo->undos) = 1;
               (modo->bombas) = ((*fil)*(*col))*90 / 100;
               (modo->cantFlags) = ((*fil)*(*col))*90 / 100;
            } else {
               printf("Este modo de juego es solo para tableros de mas de 100 casilleros.\n");
               dificultad = 0;
            }
            break;
         }
      }
   }
   return;
}

void
imprimeTablero(char*** matriz, int fil, int col) {
   int i, j;

   for (i = 0; i < fil; i++) {
      for (j = 0; j < col; j++) {
         if (i == 0)
            printf("%d\t", (*matriz)[i][j]);
         else
            printf("%c\t", (*matriz)[i][j]);
      }
      printf("\n");
   }
   return;
}

