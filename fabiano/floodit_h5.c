#include <stdlib.h>
#include <stdio.h>
#include "mapa.h"


int main(int argc, char **argv) {
  int i;
  int cor;
  int dist;
  int fpos;
  int *ncorfront;
  tmapa m, *dini, *dmax;
  tfronteira *f;
  tplano *p;

  carrega_mapa(&m);

  f = aloca_fronteira(&m);
  p = aloca_plano(&m);
  dini = aloca_mapa(&m);
  dmax = aloca_mapa(&m);
 
  ncorfront = (int*) malloc((m.ncores + 1) * sizeof(int));
  cor = m.mapa[0][0];
  fronteira_mapa(&m, f);
  
  while(f->tamanho > 0) {
    distancia_mapa(&m, dini, 0, 0);
    distancia_mapa(&m, dmax, dini->lmax, dini->cmax);
    for(i = 1; i <= m.ncores; i++)
      ncorfront[i] = 0;
    for(i = 0; i < f->tamanho; i++)
      ncorfront[m.mapa[f->pos[i].l][f->pos[i].c]]++;
    dist = dmax->ncores + 1;
    fpos = 0;
    for(i = 0; i < f->tamanho; i++)
      if(dmax->mapa[f->pos[i].l][f->pos[i].c] < dist) {
        dist = dmax->mapa[f->pos[i].l][f->pos[i].c];
	fpos = i;
      }
      else if(dmax->mapa[f->pos[i].l][f->pos[i].c] == dist &&
	      ncorfront[m.mapa[f->pos[i].l][f->pos[i].c]] >
	      ncorfront[m.mapa[f->pos[fpos].l][f->pos[fpos].c]])
	fpos = i;
    cor = m.mapa[f->pos[fpos].l][f->pos[fpos].c];
    pinta_mapa(&m, cor);
    insere_plano(p, cor);
    fronteira_mapa(&m, f);
  }
  mostra_plano(p);
  
  return 0;
}
