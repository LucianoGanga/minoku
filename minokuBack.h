#ifndef _minokuBack_h

#define _minokuBack_h

typedef struct {
   int bombas;
   int undos;
   int cantFlags;
   int mov;
} TipoDificultad;

typedef struct {
   char x1;
   int y1;
   char x2;
   int y2;
} TipoCoordenada;

#define MAXFIL	19
#define MAXCOL	19
#define MINFIL	5
#define MINCOL	5

#define BLOQUE 10
#define CANTSTRINGS 7
#define QUERYFILA 0
#define QUERYCOL 1
#define SI 0
#define NO 1

#define IMPRIME_INSTRUCCIONES 1
#define DEBUG_MODE 1

enum {
   UNDO, FLAG, UNFLAG, FLAGLINE, UNFLAGLINE, BARRER
};

enum {
   SIGUE, PERDIO, GANO
};




/*************************/
/* Funciones del BackEnd */
/*************************/

void seteaModo(TipoDificultad *, int, int, int);

void generaTableros(char***, char***, int, int, int);

void sizeTablero(char***, int, int);

void poneBombas(char ***, int, int, int);

int fflag(char***, int, int, char, int, int *, int*, int*);

int unflag(char***, int, int, char, int, int*, int*, int*);

int flagline(char***, int, int, char, int, char, char, int*, int*, int*);

int unflagline(char***, int, int, char, int, char, char, int*, int*, int*);


void juegoCampania();

void cargaJuego(char * nombreArchivo);

void cierraJuego();

/* Imprime el tablero */
void imprimeTablero(char***matriz, int fil, int col);


/*************************/
/* Funciones del FrontEnd */
/*************************/

/* Inicializa el juego */
void initMinoku();

/* Genera el nivel 1 del menu */
void menu();

/* Genera el nivel 2 del menu */
void submenu(int nroMenu);

/* Limpia la consola */
void limpiaPantalla();

/* Imprime los comandos del juego */
void imprimeInstrucciones();

/* Le pide al usuario el nivel de dificultad */
int pideNivelDificultad(int fil, int col);

/* Le pide al usuario las dimensiones de la matriz */
void pideDimensiones(int *fil, int *col);

/* Le pide al usuario que escriba un comando y devuelve el indice del mismo */
int escaneaComando();

/* Le */
int queryCol(char***, int, int, int);
int queryFil(char***, int, int, char);

int escanear(char*** , char*** , int , int , TipoDificultad *, int *, TipoCoordenada *);
#endif
