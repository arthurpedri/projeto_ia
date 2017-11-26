#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "grafo.h"


int main(int argc, char **argv) {
  char turno;
  int k; // Tamanho do jogo;
  char l;
  scanf("%c %d", &c, &k);
  grafo g = gera_grafo(k, l);
  while () {
    popula_grafo(g);
    // Avalia as jogadas com min max
    // retorna a melhor jogada;
    // destroi esses vértices
    // espera entrada
  }


  return 0;
}
