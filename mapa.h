#include <stdlib.h>

typedef struct {
  int nlinhas;
  int ncolunas;
  int ncores;
  int **mapa;
} tmapa;

void gera_mapa(tmapa *m, int semente);


void carrega_mapa(tmapa *m);

void mostra_mapa(tmapa *m);

void mostra_mapa_cor(tmapa *m);

void pinta(tmapa *m, int l, int c, int fundo, int cor) ;

void pinta_mapa(tmapa *m, int cor) ;
