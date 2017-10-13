#include <stdlib.h>
#include <stdio.h>
#include "mapa.h"


int main(int argc, char **argv) {
  int i;
  int cor;
  int dist;
  int ncor;
  int *ncorfront;
  int *distfront;
  tmapa m, *dini, *dmax;
  tfronteira *f;
  tplano *p;

  carrega_mapa(&m);

  f = aloca_fronteira(&m);
  p = aloca_plano(&m);
  dini = aloca_mapa(&m);
  dmax = aloca_mapa(&m);
 
  ncorfront = (int*) malloc((m.ncores + 1) * sizeof(int));
  distfront = (int*) malloc((m.ncores + 1) * sizeof(int));
  cor = m.mapa[0][0];
  fronteira_area_mapa(&m, f);
  
  while(f->tamanho > 0) {
    distancia_mapa(&m, dini, 0, 0);
    distancia_mapa(&m, dmax, dini->lmax, dini->cmax);
    for(i = 1; i <= m.ncores; i++) {
      ncorfront[i] = 0;
      distfront[i] = dmax->ncores;
    }
    for(i = 0; i < f->tamanho; i++) {
      ncorfront[m.mapa[f->pos[i].l][f->pos[i].c]] += f->pos[i].v;
      if(distfront[m.mapa[f->pos[i].l][f->pos[i].c]] >
	 dmax->mapa[f->pos[i].l][f->pos[i].c])
	distfront[m.mapa[f->pos[i].l][f->pos[i].c]] =
	  dmax->mapa[f->pos[i].l][f->pos[i].c];
    }
    dist = dmax->ncores + 1;
    ncor = 0;
    for(i = 1; i <= m.ncores; i++)
      if(ncorfront[i] > 0)
	if(distfront[i] < dist || (distfront[i] == dist && ncorfront[i] > ncor)) {
	  dist = distfront[i];
	  ncor = ncorfront[i];
	  cor = i;
	}
    pinta_mapa(&m, cor);
    insere_plano(p, cor);
    fronteira_area_mapa(&m, f);
  }
  mostra_plano(p);
  
  return 0;
}
