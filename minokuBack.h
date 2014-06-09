#ifndef _minokuBack_h

#define _minokuBack_h

typedef struct {
   int bombas;
   int undos;
   int cantFlags;
   int mov;
   int nivelDificultad;
   int campania;
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
#define NOFLAG 2
#define TIPO_TABLERO_VISIBLE 0
#define TIPO_TABLERO_OCULTO 1


#define IMPRIME_INSTRUCCIONES 0
#define DEBUG_MODE 1

/* MENSAJES DE ERROR */
#define ERROR_APERTURA_ARCHIVO -1
#define ERROR_LECTURA_ARCHIVO -2
#define ERROR_ESCRITURA_ARCHIVO -3


#define VACIAR_BUFFER while (getchar() != '\n')


enum {
   UNDO, FLAG, UNFLAG, FLAGLINE, UNFLAGLINE, BARRER, HAYBOMBA, NADA
};

enum {
   SIGUE, PERDIO, GANO
};


/*************************/
/* Funciones del BackEnd */
/*************************/

void seteaModo(TipoDificultad *, int, int);

void generaTableros(char***, char***, int, int, int);

void sizeTablero(char***, int, int, int);

void poneBombas(char ***, int, int, int);

int barrer(char***, char***, int, int, char, int, int *, int *);

int fflag(char***, int, int, char, int, int *, int*, int*);

int unflag(char***, int, int, char, int, int*, int*, int*);

int flagline(char***, int, int, char, int, char, char, int*, int*, int*);

int unflagline(char***, int, int, char, int, char, char, int*, int*, int*);

int cargaJuego(char * nombreArchivo, char***, char***, TipoDificultad *, int *, int *);

int guardaJuego(char * nombreArchivo, char***, char***, TipoDificultad *, int, int);

void cierraJuego();

int undo(char***, int, int, TipoCoordenada *, int *, int *, int*, int *);

/* Evalua si las condiciones de juego estan dadas para que termine la partida */
int estadoDeJuego(char***, char***, int, int, TipoDificultad *);

#endif
