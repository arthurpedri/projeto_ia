#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "grafo.h"


int main(int argc, char **argv) {
  int cor;
  tmapa m;
  int semente;
  
  int i, j;

  scanf("%d", &(m.nlinhas));
  scanf("%d", &(m.ncolunas));
  scanf("%d", &(m.ncores));
  m.mapa = (int**) malloc(m.nlinhas * sizeof(int*));
  for(i = 0; i < m.nlinhas; i++) {
    m.mapa[i] = (int*) malloc(m.ncolunas * sizeof(int));
    for(j = 0; j < m.ncolunas; j++)
      scanf("%d", &(m.mapa[i][j]));
  }
  
  gera_mapa(&m, semente);
  //mostra_mapa_cor(&m);
  
  lista resultado = A_estrela_simples(&m);

  printf("%d\n", tamanho_lista(resultado));
  imprime_lista(resultado);


  return 0;
}
