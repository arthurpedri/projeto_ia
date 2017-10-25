#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "grafo.h"
#include "lista.h"
#include "mapa.h"


int main(int argc, char **argv) {
  int cor;
  tmapa m;
  int semente;

  if(argc < 4 || argc > 5) {
    printf("uso: %s <numero_de_linhas> <numero_de_colunas> <numero_de_cores> [<semente_aleatoria>]\n", argv[0]);
    exit(1);
  }

  m.nlinhas = atoi(argv[1]);
  m.ncolunas = atoi(argv[2]);
  m.ncores = atoi(argv[3]);

  if(argc == 5)
    semente = atoi(argv[4]);
  else
    semente = -1;
  gera_mapa(&m, semente);
  mostra_mapa_cor(&m);

  scanf("%d", &cor);
  while(cor > 0 && cor <= m.ncores) {
    pinta_mapa(&m, cor);
    mostra_mapa_cor(&m); // para mostrar sem cores use mostra_mapa(&m);
    scanf("%d", &cor);
  }

  return 0;
}
