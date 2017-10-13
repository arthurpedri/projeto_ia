#include <stdlib.h>
#include <stdio.h>
#include "mapa.h"

long int estados = 0;

void floodit(tmapa *m, tplano *p, tplano *pmin, int *corte) {
  int i;
  int cor;
  tfronteira *f;
  tmapa *mnovo;

  if(p->passos > *corte)
    return;
  f = empilha_fronteira();
  fronteira_mapa_cores(m, f);
  if(!f->tamanho) {
    p->solucao = 1;
    desempilha_fronteira();
    return;
  }
  mnovo = empilha_mapa();
  for(i = 0; i < f->tamanho; i++) {
    cor = m->mapa[f->pos[i].l][f->pos[i].c];
    copia_mapa(m, mnovo);
    pinta_mapa(mnovo, cor);
    insere_plano(p, cor);
    estados++;
    floodit(mnovo, p, pmin, corte);
    if(p->solucao && (!pmin->passos || p->passos < pmin->passos)) {
      copia_plano(p, pmin);
      *corte = pmin->passos;
      mostra_plano(pmin);
    }
    remove_plano(p);
    p->solucao = 0;
  }
  desempilha_fronteira();
  desempilha_mapa();
}

int main(int argc, char **argv) {
  tmapa m;
  tplano *p, *pmin;
  int semente;
  int corte;

  if(argc < 5 || argc > 6) {
    printf("uso: %s <numero_de_linhas> <numero_de_colunas> <numero_de_cores> <tamanho_max_plano> [<semente_aleatoria>]\n", argv[0]);
    exit(1);
  }

  m.nlinhas = atoi(argv[1]);
  m.ncolunas = atoi(argv[2]);
  m.ncores = atoi(argv[3]);
  corte = atoi(argv[4]);

  if(argc == 6)
    semente = atoi(argv[5]);
  else
    semente = 0;
  gera_mapa(&m, semente);

  aloca_pilhas(&m);
  
  p = aloca_plano(&m);
  pmin = aloca_plano(&m);

  floodit(&m, p, pmin, &corte);

  simula_plano(&m, pmin);
  mostra_plano(pmin);
  
  return 0;
}
