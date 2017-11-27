#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "grafo.h"


int main(int argc, char **argv) {
  char turno;
  int k; // Tamanho do jogo;
  char l;
  scanf("%c %d", &l, &k);
  grafo g = gera_grafo(k, l);
  while (1) {
    imprime_debug("MAIN\n");
    popula_grafo(g);
    // Avalia as jogadas com min max (retorna a melhor jogada)
    minmax(g);

    // destroi esses vértices
    destroi_grafo(g);
    // espera entrada
    scanf("%c %d", &l, &k);
    g = gera_grafo(k, l);
  }


  return 0;
}
