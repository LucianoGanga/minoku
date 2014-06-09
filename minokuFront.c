#include <stdio.h>
#include "getnum.h"
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include "minokuBack.h"

/*************************/
/* Funciones del FrontEnd */
/*************************/

/* Inicializa el juego */
void initMinoku();

/* Genera el nivel 1 del menu */
void menu();

/* Genera el nivel 2 del menu */
void submenu(int);

/* Limpia la consola */
void limpiaPantalla();

/* Imprime el tablero */
void imprimeTablero(char ***, int, int);

/* Imprime los comandos del juego */
void imprimeInstrucciones();

/* Le pide al usuario el nivel de dificultad */
int pideNivelDificultad(int, int);

/* Le pide al usuario las dimensiones de la matriz */
void pideDimensiones(int *, int *);

/* Le pide al usuario que escriba un comando y devuelve el indice del mismo */
int escaneaComando();

/* Le pide al usuario que escriba un comando y devuelve el indice del mismo */
int procesaComando(int, char***, char***, int, int, TipoDificultad *, int *, TipoCoordenada *);

/* Segun el nro que ingresa por parametro, evalua el estado del juego para saber que hay que mostrar y hacer */
int evaluaEstadoJuego(int, TipoDificultad *);

/* Muestra cuantos flags, undos y movimientos (en caso de juego con limite de mov) quedan  */
void imprimeParametros(int, int, int);

/* Inicia un juego individual, con o sin limite de movimientos (depende del parametro) */
void juegoIndividual(int);

/* Inicia un de tipo campania */
void juegoCampania();

/* Inicia una partida */
int jugar(char***, char***, int, int, TipoDificultad *, int);

/* Inicia una partida en modo campania*/
int jugarCampania(FILE **, TipoDificultad *, char ***, char ***, int, int);

/* Le */
int queryCol(char***, int, int, int);
int queryFil(char***, int, int, char);

/* Carga el archivo de la campania */
int cargaCampania(FILE **);

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

void submenu(int nroMenu) {
   int opcion;

   switch (nroMenu) {
      case 1:
         printf("Ingrese el modo de juego: \n");
         printf("1) Juego solo sin limite de movimientos.\n");
         printf("2) Juego individual con limite de movimientos.\n");
         printf("3) Juego por campania.\n");

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
         char nombreArchivo[16];
         int tipoJuego;
         int fil, col;
         char **tablero = NULL, **tableroOculto = NULL;
         TipoDificultad modo;

         printf("Escriba el nombre de la partida que desea cargar: \n");
         scanf("%15s", nombreArchivo);

         tipoJuego = cargaJuego(nombreArchivo, &tablero, &tableroOculto, &modo, &fil, &col);

         /* Si es un juego individual (con o sin limite de movimientos) */
         if (DEBUG_MODE)
            printf("Tipo de juego es: %d\n", tipoJuego);

         if (tipoJuego == 3) {
            FILE * archivo;
            /* Cargo el archivo de campania */
            cargaCampania(&archivo);

            jugarCampania(&archivo, &modo, &tablero, &tableroOculto, fil, col);

         } else
            jugar(&tablero, &tableroOculto, fil, col, &modo, 0);
      }
         break;
      case 3:
      {
         /* @TODO: Cambiar esto por una funcion que de opcion "S" y "N" para salir del juego o volver al menu principal */
         int salir = -1;
         salir = getint("Esta seguro que desea salir?: \n");

         if (salir == 1)
            cierraJuego();
         else
            limpiaPantalla();
         menu();
      }
         break;

   }
}

void juegoIndividual(int opcion) {
   int fil, col;
   char **tablero = NULL, **tableroOculto = NULL;
   TipoDificultad modo;
   pideDimensiones(&fil, &col);
   modo.nivelDificultad = pideNivelDificultad(fil, col);
   modo.campania = 0;

   if (opcion == 1) {
      printf("Eligio juego individual sin limite de movimientos");
      modo.mov = -1;
      seteaModo(&modo, fil, col);
   } else {
      printf("Eligio juego individual con limite de movimientos");
      seteaModo(&modo, fil, col);
      modo.mov = modo.bombas + modo.undos;
   }

   /*printf("fil %d col %d\n", fil, col);*/

   generaTableros(&tablero, &tableroOculto, fil, col, modo.bombas);

   /*imprimeTablero(&tablero, fil, col);*/
   printf("\n\n");
   /*imprimeTablero(&tableroOculto, fil, col);*/

   jugar(&tablero, &tableroOculto, fil, col, &modo, 0);

}

void
juegoCampania() {
   char **tablero = NULL, **tableroOculto = NULL;
   TipoDificultad modo;
   FILE * archivo;

   /* Cargo el archivo de campania */
   cargaCampania(&archivo);

   /* Seteo las variables base de la campania */
   modo.campania = 1;
   modo.mov = -1;


   /* Empiezo el modo campania */
   jugarCampania(&archivo, &modo, &tablero, &tableroOculto, 0, 0);
}

void pideDimensiones(int * fil, int * col) {
   limpiaPantalla();

   while (*fil < MINFIL || *fil > MAXFIL) {
      *fil = getint("¿Cuantas filas tendra el tablero? ");
      if (*fil < MINFIL || *fil > MAXFIL)
         printf("La cantidad de filas debe ser entre 5 y 19.\n");
   }

   limpiaPantalla();

   while (*col < MINCOL || *col > MAXCOL) {
      *col = getint("¿Cuantas columnas tendra el tablero? ");
      if (*col < MINCOL || *col > MAXCOL)
         printf("La cantidad de columnas debe ser entre 5 y 19.\n");
   }
   return;
}

int pideNivelDificultad(int fil, int col) {
   /* Nivel de dificultad - Pide filas y columnas para saber si se ingreso un nivel correcto */
   int nivelDificultad;

   printf("Escoja la dificultad:\n");
   printf("1) Facil\n");
   printf("2) Medio\n");
   printf("3) Dificil\n");
   printf("4) Pesadilla\n\n");
   /* Pido al usuario que ingrese la opcion */
   nivelDificultad = getint("");


   if (nivelDificultad < 1 || nivelDificultad > 4) {
      printf("La opcion ingresada no es valida. \n\n");
      nivelDificultad = pideNivelDificultad(fil, col);
   }

   if (nivelDificultad == 4 && (fil)*(col) < 100) {
      printf("Este modo de juego es solo para tableros de mas de 100 casilleros.\n");
      nivelDificultad = pideNivelDificultad(fil, col);
   }
   limpiaPantalla();

   return nivelDificultad;
}

void limpiaPantalla() {
   printf("\033[2J\033[1;1H");
   return;
}

void imprimeTablero(char*** matriz, int fil, int col) {
   int i, j;
   if (DEBUG_MODE)
      printf("Imprimiendo tablero...\n");

   for (i = 0; i < fil + 1; i++) {
      for (j = 0; j < col + 1; j++) {
         if (i == 0)
            printf("%d\t", j);
         else if (j == 0)
            printf("%c\t", i - 1 + 'A');
         else
            printf("%c\t", (*matriz)[i - 1][j - 1]);
      }
      printf("\n");
   }
   return;
   printf("\n");
}

void imprimeInstrucciones() {
   if (DEBUG_MODE == 1)
      printf("* MODO DEBUG: ON \n");

   if (IMPRIME_INSTRUCCIONES == 1) {
      printf("*************************************************************************\n");
      printf("*********************** COMANDOS VALIDOS DE MINUKU **********************\n");
      printf("* S (fila, columna): barrer el casillero (si hay una mina pierde) \n");
      printf("* unflag/flag (F,C): sacar/poner un banderín en el casillero F,C \n");
      printf("* unflag/flag (FO,CO:FD,CD): sacar/poner una serie de banderines desde\n");
      printf("el casillero origen FO,CO hasta el casillero FD,CD. No se permiten\n");
      printf("diagonales. Solo desplazamientos sobre columnas o filas.\n");
      printf("* query F o query C: devuelve la secuencia de aparición de las minas\n");
      printf("en esa determinada fila o columna. F es una letra, C es un número. \n");
      printf("* save filename: guardará el juego en un archivo de nombre filename.\n");
      printf("* quit: saldrá del juego, preguntando si desea guardar el mismo en un\n");
      printf("archivo.\n");
      printf("* undo: volverá al estado anterior. \n");
      printf("*************************************************************************\n");
      printf("*************************************************************************\n\n");
   }
   return;
}

int jugar(char*** tablero, char*** tableroOculto, int fil, int col, TipoDificultad *modo, int nivelCampania) {

   int ultimaJugada = UNDO, fin = SIGUE, estadoJuego = SIGUE, indiceComando, estadoProcesaComando;
   TipoCoordenada coord;

   /* Una vez que tengo los datos necesarios para comenzar el juego, inicio un
    * bucle de juego hasta que la variable "fin" cambie su valor.
    */

   /* Una vez que tengo los datos necesarios para comenzar el juego, limpio la pantalla y comienzo una partida */
   limpiaPantalla();
   imprimeInstrucciones();

   if (modo->campania == 1)
      printf("Esta jugando el nivel %d del modo campania.\n", nivelCampania);

   /* Imprimo el tablero para que lo vea el usuario */
   imprimeTablero(tablero, fil, col);

   if (DEBUG_MODE == 1) /* Imprimo el tablero oculto */ {
      printf("\n");
      imprimeTablero(tableroOculto, fil, col);
   }

   imprimeParametros(modo->undos, modo->cantFlags, modo->mov);


   /* Inicio un bucle de juego hasta que la variable "fin" cambie su valor. */

   while (fin == SIGUE) {
      /* ------- Comienzo de turno ------- */

      /* Inicio el escaneo de comandos */
      indiceComando = escaneaComando();



      estadoProcesaComando = NO;
      /* Ejecuto el comando que introdujo el usuario */
      while (estadoProcesaComando == NO) {
         estadoProcesaComando = procesaComando(indiceComando, tablero, tableroOculto, fil, col, modo, &ultimaJugada, &coord);
         if (estadoProcesaComando == NO)
            indiceComando = escaneaComando();
      }


      /* Me fijo si termino la partida en caso de barrer una bomba */
      estadoJuego = estadoDeJuego(tablero, tableroOculto, fil, col, modo);
      if (estadoJuego == HAYBOMBA) {
         int n, resp = 1;
         limpiaPantalla();
         printf("\nUsted ha barrido una bomba.\n\n");
         imprimeTablero(tablero, fil, col);
         printf("\nQue desea hacer?:\n");
         printf("1) Hacer un undo\n");
         printf("2) Terminar\n");
         while (resp) {
            n = getint("");
            switch (n) {
               case 1:
               {
                  undo(tablero, fil, col, &coord, &(modo->mov), &(modo->cantFlags), &(modo->undos), &ultimaJugada);
                  resp = 0;
                  estadoJuego = SIGUE;
                  break;
               }
               case 2:
               {
                  estadoJuego = PERDIO;
                  resp = 0;
                  break;
               }
            }
         }
      }

      limpiaPantalla();
      fin = evaluaEstadoJuego(estadoJuego, modo);

      /* Imprimo el tablero para que lo vea el usuario */
      imprimeTablero(tablero, fil, col);

      if (DEBUG_MODE == 1) /* Imprimo el tablero oculto */ {
         printf("\n");
         imprimeTablero(tableroOculto, fil, col);
      }


      imprimeParametros(modo->undos, modo->cantFlags, modo->mov);

      /* ------- Fin de turno ------- */
   }

   printf("\nEste es el tablero original\n\n");
   imprimeTablero(tableroOculto, fil, col);

   return 0;
}

int jugarCampania(FILE ** archivo, TipoDificultad *modo, char *** tablero, char *** tableroOculto, int fil, int col) {
   int resultadoPartida = SIGUE;
   int filasCampania, columnasCampania, nivelDificultadCampania;
   int nivelCampania = 0;

   while (!feof(*archivo) && (resultadoPartida == SIGUE || resultadoPartida == GANO)) {
      nivelCampania++;
      /* Escaneo la linea buscando los parametos de este nivel */
      fscanf(*archivo, "%d\t%dx%d", &nivelDificultadCampania, &filasCampania, &columnasCampania);

      /* Me fijo si es una campania nueva o una que estoy cargando desde una partida guardada */
      /* Tambien me fijo de mostrar el nivel que corresponde de la campania, no uno anterior */
      if (!((fil > 0 && col > 0) && (filasCampania != fil || columnasCampania != col || modo->nivelDificultad != nivelDificultadCampania))) {

         modo->nivelDificultad = nivelDificultadCampania;

         /*  Con todos los datos, seteo modo y genero los tableros,
          *  SOLO si es una campania nueva 
          */
         if (fil == 0 && col == 0) {
            printf("Filas: %d\n", filasCampania);
            printf("Columnas: %d\n", columnasCampania);
            printf("Bombas: %d\n", modo->bombas);

            seteaModo(modo, filasCampania, columnasCampania);
            generaTableros(tablero, tableroOculto, filasCampania, columnasCampania, modo->bombas);
         }

         /* Inicio esta partida */
         resultadoPartida = jugar(tablero, tableroOculto, filasCampania, columnasCampania, modo, nivelCampania);
      }

   }
   return resultadoPartida;
}

int evaluaEstadoJuego(int estadoJuego, TipoDificultad * modo) {
   switch (estadoJuego) {
      case PERDIO:
         printf("Perdiste!   :(\n\n");
         if (modo->mov == 0)
            printf("Se ha quedado sin movimientos\n");
         break;
      case GANO:
      {
         int opcion = 0;
         printf("Felicitaciones! Ha resuelto este tablero :)\n\n");

         if (modo->campania == 1) {
            while (opcion != 1 && opcion != 2) {
               printf("Que desea hacer?\n");
               printf("1) Jugar el siguiente nivel de campania\n");
               printf("2) Salir del juego\n");
               opcion = getint("");
               if (opcion != 1 && opcion != 2)
                  printf("Opción incorrecta. Intente nuevamente. ");
            }
         }
      }
         break;
      case HAYBOMBA:
         printf("Hay una bomba. Desea volver atras y seguir jugando?\n");
         if (1 == 1) /* Si el usuario no quiso seguir jugando */
            return PERDIO;

         break;
   }
   return SI;
}

void imprimeParametros(int undo, int flags, int mov) {
   printf("\nCantidad de undos:\t%d.\nCantidad de flags:\t%d.\n", undo, flags);
   if (mov > 0)
      printf("Cantidad de movimientos restantes:\t%d\n\n", mov);
   else
      printf("\n");

   return;
}

int queryCol(char*** tableroOculto, int fil, int col, int columna) {
   int i, b = 0;
   printf("\n\n");
   columna--;
   if (columna < 0 || columna > col)
      return NO;

   printf("Respuesta de su query: ");
   for (i = 0; i < fil; i++) {
      if ((*tableroOculto)[i][columna] == '#')
         b++;
      else
         if (b != 0) {
         printf("%d\t", b);
         b = 0;
      }
   }
   if (b != 0)
      printf("%d\n\n", b);
   else
      printf("\n\n");

   return SI;
}

int queryFil(char*** tableroOculto, int fil, int col, char fila) {
   int i, b = 0;
   printf("\n\n");

   if ((toupper(fila) - 'A') < 0 || (toupper(fila) - 'A') > fil)
      return NO;

   printf("Respuesta de su query: ");
   for (i = 0; i < col; i++) {
      if ((*tableroOculto)[toupper(fila) - 'A'][i] == '#') {
         b++;
      } else
         if (b != 0) {
         printf("%d\t", b);
         b = 0;
      }
   }
   if (b != 0)
      printf("%d\n\n", b);
   else
      printf("\n\n");

   return SI;
}

int escaneaComando() {
   char *string = NULL, *aux = NULL;
   char *biblioStrings[CANTSTRINGS] = {"S", "query", "flag", "unflag", "save", "quit", "undo"};
   int i, flag = 0, c;
   int escanieComando = 0;
   printf("Ingrese comando: \n");
   while (flag == 0) {
      i = 0;
      while (!isspace(c = getchar())) {
         escanieComando = 1;
         if (i % BLOQUE == 0) {
            aux = realloc(string, (i + BLOQUE) * sizeof (*string));
            if (aux == NULL) {
               free(string);
               printf("No hay memoria.\n");
               return 0;
            }
            string = aux;
         }
         string[i] = c;
         i++;
      }
      string = realloc(string, i * sizeof (*string));
      string[i] = 0;

      for (i = 0; i < CANTSTRINGS && flag == 0; i++) {
         if (strcmp(string, biblioStrings[i]) == 0) {
            flag = 1;
         }
      }
      i--;

      if (flag == 0 && escanieComando == 1)
         printf("No se reconoce el comando. Intente nuevamente: \n");

   }
   return i;
}

int procesaComando(int i, char*** tablero, char*** tableroOculto, int fil, int col, TipoDificultad *modo, int *ultimaJugada, TipoCoordenada * coord) {
   int c, d;
   int parametros, tipoDeQuery, respuesta;

   switch (i) {
      case 0: /* comando "S" */
      {
         parametros = scanf("(%c,%d", &(coord->x1), &(coord->y1));

         c = getchar();
         if (c != '\n')
            d = getchar();
         if ((parametros == 2) && c == ')' && d == '\n') {
            if (barrer(tablero, tableroOculto, fil, col, coord->x1, coord->y1, &(modo->mov), ultimaJugada) == NO) {
               printf("Parametros inexistentes\n");
            }

         } else {
            printf("No se reconoce el comando\n");
            return NO;
         }
         break;
      }
      case 1: /* comando "query" */
      {
         parametros = scanf("%d", &(coord->y1));

         if (parametros != 1) {
            c = getchar();
            tipoDeQuery = QUERYFILA;
         } else
            tipoDeQuery = QUERYCOL;
         if (tipoDeQuery == QUERYFILA && isalpha(c)) {
            d = getchar();
            if (d == '\n') {
               if (queryFil(tableroOculto, fil, col, c) == NO)
                  printf("La fila no existe en el tablero.\n");
            } else {
               printf("No se reconoce el comando\n");
               return NO;
            }
         } else if (tipoDeQuery == QUERYCOL && (d = getchar()) == '\n') {
            if (queryCol(tableroOculto, fil, col, coord->y1) == NO)
               printf("La columna no existe en el tablero.\n");
         } else {
            printf("No se reconoce el comando\n");
            return NO;
         }
         break;
      }
      case 2: /* comando "flag" */
      case 3: /* comando "unflag" */
      {
         parametros = scanf("(%c,%d:%c,%d", &(coord->x1), &(coord->y1), &(coord->x2), &(coord->y2));

         c = getchar();
         if (c != '\n')
            d = getchar();

         if ((parametros == 2) && c == ')' && d == '\n') {
            if (i == 2) {
               respuesta = fflag(tablero, fil, col, coord->x1, coord->y1, &(modo->mov), &(modo->cantFlags), ultimaJugada);
            } else {
               respuesta = unflag(tablero, fil, col, coord->x1, coord->y1, &(modo->mov), &(modo->cantFlags), ultimaJugada);
            }
         } else if ((parametros == 4) && c == ')' && d == '\n') {
            if (i == 2) {
               respuesta = flagline(tablero, fil, col, coord->x1, coord->y1, coord->x2, coord->y2, &(modo->mov), &(modo->cantFlags), ultimaJugada);
            } else {
               respuesta = unflagline(tablero, fil, col, coord->x1, coord->y1, coord->x2, coord->y2, &(modo->mov), &(modo->cantFlags), ultimaJugada);
            }
         } else {
            printf("No se reconoce el comando\n");
            return NO;
         }

         if (respuesta == NO)
            printf("Parametros inexistentes\n");
         else if (respuesta == NOFLAG)
            printf("%s\n", parametros == 2 ? "No quedan mas flags." : "La cantidad de flags a poner es mayor a la cantidad de flags que quedan");


         break;
      }
      case 4: /* comando "save" */
      {
         int estadoGuardado;
         char nombreArchivo[16];
         scanf("%15s", nombreArchivo);

         estadoGuardado = guardaJuego(nombreArchivo, tablero, tableroOculto, modo, fil, col);

         if (estadoGuardado >= 0)
            printf("Su partida ha sido guardada con exito.\n");

         return estadoGuardado;
         break;
      }
      case 5: /* comando "quit" */
      {
         int opcion = 0;
         char nombreArchivo[16];

         printf("Esta seguro que desea salir?\n");
         printf("1) Si, salir y guardar la partida.\n");
         printf("2) Si, salir pero no quiero guardar.\n");
         printf("3) No, quiero seguir jugando.\n");

         while (opcion < 1 || opcion > 3) {
            opcion = getint("");
            if (opcion < 1 || opcion > 3)
               printf("La opcion seleccionada es incorrecta. Intente nuevamente!\n");
         }
         switch (opcion) {
               /* Guardo el juego */
            case 1:
               printf("Con que nombre desea guardar la partida? ");
               scanf("%15s", nombreArchivo);

               guardaJuego(nombreArchivo, tablero, tableroOculto, modo, fil, col);
               break;
               /* Sigo jugando*/
            case 3:
               limpiaPantalla();
               return SI;
               break;
         }

         /* Finalizo Minoku */

         printf("Hasta luego!\n");

         cierraJuego();
         break;
      }
      case 6: /* comando "undo" */
      {
         if (undo(tablero, fil, col, coord, &(modo->mov), &(modo->cantFlags), &(modo->undos), ultimaJugada) == NO)
            printf("No se pueden hacer dos undos seguidos.\n");

         break;
      }
   }
   return SI;
}

int
cargaCampania(FILE ** archivo) {
   char nombreArchivo[20] = "campaniaSimple.txt";

   if (DEBUG_MODE)
      printf("Cargando la campania %s\n", nombreArchivo);

   *archivo = fopen(nombreArchivo, "rt");
   if (*archivo == NULL)
      return ERROR_APERTURA_ARCHIVO;

}

