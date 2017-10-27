#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "grafo.h"


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
  
  lista resultado = A_estrela(&m);
  
  // lista resultado = constroi_lista();
  // int *um, *dois;
  // um = malloc(sizeof(int));
  // dois = malloc(sizeof(int));
  
  // *um = 1;
  // *dois = 2;
  
  
  // insere_lista(um, resultado);
  // no aux = primeiro_no(resultado);
  // int *cont = conteudo(aux);
  // printf("1 %d\n", *cont);
  
  // insere_lista(um, resultado);
  
  // aux = proximo_no(aux);
  // cont = conteudo(aux);
  
  // printf("2 %d\n", *cont);
  // #ifndef DEBUG
  // printf("nossa func\n");
  // #endif
  
  printf("%d\n", tamanho_lista(resultado));
  imprime_lista(resultado);
  /*

  scanf("%d", &cor);
  while(cor > 0 && cor <= m.ncores) {
    pinta_mapa(&m, cor);
    mostra_mapa_cor(&m); // para mostrar sem cores use mostra_mapa(&m);
    scanf("%d", &cor);
  }*/

  return 0;
}
