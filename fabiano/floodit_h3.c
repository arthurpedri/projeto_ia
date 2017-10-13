#include <stdlib.h>
#include <stdio.h>
#include "mapa.h"


int main(int argc, char **argv) {
  int i;
  int cor;
  int dist;
  int pos;
  tmapa m, *d, *dsoma;
  tfronteira *f;
  tplano *p;

  carrega_mapa(&m);

  f = aloca_fronteira(&m);
  p = aloca_plano(&m);
  d = aloca_mapa(&m);
  dsoma = aloca_mapa(&m);
 
  cor = m.mapa[0][0];
  fronteira_mapa(&m, f);
  
  while(f->tamanho > 0) {
    distancia_mapa(&m, d, m.nlinhas - 1, m.ncolunas - 1);
    for(i = 0; i < d->nlinhas - 1; i++) {
      distancia_mapa(&m, d, i, m.ncolunas - 1);
      soma_mapa(d, dsoma);
    }
    for(i = 0; i < d->ncolunas - 1; i++) {
      distancia_mapa(&m, d, m.nlinhas - 1, i);
      soma_mapa(d, dsoma);
    }
    dist = dsoma->ncores;
    pos = 0;
    for(i = 0; i < f->tamanho; i++)
      if(dsoma->mapa[f->pos[i].l][f->pos[i].c] < dist) {
        dist = dsoma->mapa[f->pos[i].l][f->pos[i].c];
	pos = i;
      }
    cor = m.mapa[f->pos[pos].l][f->pos[pos].c];
    pinta_mapa(&m, cor);
    insere_plano(p, cor);
    fronteira_mapa(&m, f);
  }
  mostra_plano(p);
  
  return 0;
}
  
  
