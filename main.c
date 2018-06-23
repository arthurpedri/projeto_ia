#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "grafo.h"
#include "campo.h"


int main(int argc, char **argv) {
  char buf[512];
  char campo[512];
  char turno;
  char lado_adv;
  char mov_adv;
  int k; // Tamanho do jogo;
  char l;
  // conecta com o controlador do campo
  campo_conecta(argc, argv);

  // recebe o campo inicial e o movimento do adversario
  campo_recebe(buf);

  sscanf(strtok(buf, " \n"), "%c", &l);
  sscanf(strtok(NULL, " \n"), "%d", &k);
  sscanf(strtok(NULL, " \n"), "%s", campo);
  sscanf(strtok(NULL, " \n"), "%c", &lado_adv);
  sscanf(strtok(NULL, " \n"), "%c", &mov_adv);

  // scanf("%c %d", &l, &k);
  grafo g = gera_grafo(k, l, &campo);
  while (1) {
    popula_grafo(g);
    // Avalia as jogadas com min max (retorna a melhor jogada)
    minmax(g);

    // destroi esses vértices
    destroi_grafo(g);
    // espera entrada
    campo_recebe(buf);
    // scanf("%c %d", &l, &k);
    sscanf(strtok(buf, " \n"), "%c", &l);
    sscanf(strtok(NULL, " \n"), "%d", &k);
    sscanf(strtok(NULL, " \n"), "%s", campo);
    sscanf(strtok(NULL, " \n"), "%c", &lado_adv);
    sscanf(strtok(NULL, " \n"), "%c", &mov_adv);
    
    g = gera_grafo(k, l, &campo);
  }


  return 0;
}
