#ifndef _minokuBack_h

#define _minokuBack_h

typedef struct {
   int bombas;
   int undos;
   int cantFlags;
   int mov;
} TipoDificultad;

#define MAXFIL	19
#define MAXCOL	19
#define MINFIL	5
#define MINCOL	5

void generaTableros(char***, char***, int, int, int);

void sizeTablero(char***, int, int);

void poneBombas(char ***, int, int, int);

#endif
