#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"
#include "campo.h"


int ALT_MAX;

struct vertice
{
	char *jogada;
	int *pulos;
	int nivel;
	int valor;
	int k;
	char tipojogada;
	int tamanhopulos;
	int gol;
	lista vizinhos;
};

struct grafo
{
	int k;
	char l;
	vertice raiz;
	lista vertices;
};

unsigned int n_vertices(grafo g){
	return tamanho_lista(g->vertices);
}

// void imprime_grafo(grafo g){
// 	vertice v, v2;
// 	for(no n = primeiro_no(g->vertices); n; n = proximo_no(n)){
// 		v = conteudo(n);
// 		printf("%d - [%d,%d] -> %d\n", v->cor, v->l, v->c, v->indice);
// 		for(no n2 = primeiro_no(v->vizinhos); n2; n2 = proximo_no(n2)){
// 			v2 = conteudo(n2);
// 			printf("	%d - [%d,%d]\n", v2->cor, v2->l, v2->c);
// 		}
//
// 	}
// }

void copia_vetor_int (int *dest, int *src, int tam){
	int i;
	for (i = 0; i < tam; i++){
		dest[i] = src[i];
	}
}

//------------------------------------------------------------------------------
//Aloca memoria para um vertice v
static vertice constroi_vertice(void) {
	vertice v = malloc(sizeof(struct vertice));
	if (!v)
		return NULL;
	v->nivel = 0;
	v->valor = 0;
	v->k = 0;
	v->gol = 0;
	v->vizinhos = constroi_lista();
	return v;
}

void popula_grafo (grafo g){
	vertice r_aux = g->raiz;
	vertice v;
	int *puloslocal;
	puloslocal = malloc(sizeof(int)*g->k);
	int numpulos = 0;
	if (existe_chute(r_aux)) {
		// Gera jogadas de chute
		int bolapos;
		for (bolapos = 0; bolapos < r_aux->k; bolapos++) {
			if (r_aux->jogada[bolapos] == 'o') {
				break;
			}
		}
		int indice = bolapos; // verifica posicoes direita
		// verificar direita da bola
		char ultima_jogada = ' ';
		int pode_continuar = 1;
		while ((indice != r_aux->k - 1) && (pode_continuar)){
			if (r_aux->jogada[indice + 1] == 'f'){
				// da pra procurar mais
				ultima_jogada = 'f';
			}
			else if (r_aux->jogada[indice + 1] == '.'){
				if (ultima_jogada == 'f'){
					// gerar jogada
					v = constroi_vertice();
					v->jogada = malloc(sizeof(char)*g->k);
					v->pulos = malloc(sizeof(int)*g->k);
					v->tipojogada = 'o';
					strcpy(v->jogada, r_aux->jogada);
					puloslocal[numpulos] = indice + 2;
					numpulos++;
					v->tamanhopulos = numpulos; // pulos tem que começar em 1
					copia_vetor_int(v->pulos, puloslocal, g->k);
					int limpador;
					for (limpador = bolapos; limpador <= indice; limpador++)
						v->jogada[limpador] = '.'; // limpar a posição
					v->jogada[limpador] = 'o';
					v->k = g->k;
					v->nivel = 2;
					insere_lista (v, r_aux->vizinhos);
					insere_lista(v, g->vertices);
					popula(g, v, ALT_MAX); //vertice, altura máxima
				}
				else{
					pode_continuar = 0;
				}
				ultima_jogada = '.';
			}
			indice++;

		}
		if (pode_continuar && ultima_jogada == 'f'){
			// gerar jogada de gol
			v = constroi_vertice();
			v->jogada = malloc(sizeof(char)*g->k);
			v->pulos = malloc(sizeof(int)*g->k);
			v->tipojogada = 'o';
			v->gol = 1;
			v->k = g->k;
			v->nivel = 2;
			v->valor = ((v->k)*(v->k)*2) - (v->nivel);

			if (g->l == 'd') {
				v->valor = (v->valor)*(-1);
			}
			puloslocal[numpulos] = g->k+1;
			numpulos++;
			v->tamanhopulos = numpulos; // pulos tem que começar em 1
			copia_vetor_int(v->pulos, puloslocal, g->k);
			strcpy(v->jogada, r_aux->jogada);
			int limpador;
			for (limpador = bolapos; limpador <= indice; limpador++)
				v->jogada[limpador] = '.'; // limpar a posição
			insere_lista (v, r_aux->vizinhos);
			//printf("vizinhos gol %d\n", tamanho_lista(r_aux->vizinhos));
			insere_lista(v, g->vertices);
		}

		free(puloslocal);
		puloslocal = malloc(sizeof(int)*g->k);
		numpulos = 0;
		indice = bolapos;

		// verificar esquerda da bola
		ultima_jogada = ' ';
		pode_continuar = 1;
		while ((indice != 0) && (pode_continuar)){
			if (r_aux->jogada[indice - 1] == 'f'){
				// da pra procurar mais
				ultima_jogada = 'f';
			}
			else if (r_aux->jogada[indice - 1] == '.'){
				if (ultima_jogada == 'f'){
					// gerar jogada
					v = constroi_vertice();
					v->jogada = malloc(sizeof(char)*g->k);
					v->pulos = malloc(sizeof(int)*g->k);
					v->tipojogada = 'o';
					puloslocal[numpulos] = indice;
					numpulos++;
					v->tamanhopulos = numpulos; // pulos tem que começar em 1
					copia_vetor_int(v->pulos, puloslocal, g->k);
					strcpy(v->jogada, r_aux->jogada);
					int limpador;
					for (limpador = bolapos; limpador >= indice; limpador--)
						v->jogada[limpador] = '.'; // limpar a posição
					v->jogada[limpador] = 'o';
					v->k = g->k;
					v->nivel = 2;
					insere_lista (v, r_aux->vizinhos);
					insere_lista(v, g->vertices);
					popula(g, v, ALT_MAX); //vertice, altura máxima
				}
				else{
					pode_continuar = 0;
				}
				ultima_jogada = '.';
			}
			indice--;

		}
		if (pode_continuar && ultima_jogada == 'f'){
			// gerar jogada de gol
			v = constroi_vertice();
			v->jogada = malloc(sizeof(char)*g->k);
			v->pulos = malloc(sizeof(int)*g->k);
			v->tipojogada = 'o';
			puloslocal[numpulos] = 0;
			numpulos++;
			v->k = g->k;
			v->tamanhopulos = numpulos; // pulos tem que começar em 1
			copia_vetor_int(v->pulos, puloslocal, g->k);
			v->gol = 1;
			v->nivel = 2;
			v->valor = ((v->k)*(v->k)*2) - (v->nivel);
			if (g->l == 'e') {
				v->valor = (v->valor)*(-1);
			}
			strcpy(v->jogada, r_aux->jogada);
			int limpador;
			for (limpador = bolapos; limpador >= indice; limpador--)
				v->jogada[limpador] = '.'; // limpar a posição
			insere_lista (v, r_aux->vizinhos);
			insere_lista(v, g->vertices);
		}


	}


	int i = 0;
	i = encontra_jogada(r_aux, i); // Procura a partir de i um '.' e devolve a posição dele
	//printf("i %d\n", i);
	while(i != -1){
	//imprime_debug("entrei popula_grafo\n");
		v = constroi_vertice();
		v->jogada = malloc(sizeof(char)*g->k);
		v->pulos = malloc(sizeof(int));
		v->tamanhopulos = 1; // pulos tem que começar em 1
		//printf("i+1 %d", i+1);
		v->pulos[0] = i + 1;
		strcpy(v->jogada, r_aux->jogada);
		v->tipojogada = 'f';
		v->jogada[i] = 'f'; //Coloca o jogador já na posição
		v->k = g->k;
		v->nivel = 2;
		insere_lista (v, r_aux->vizinhos);
		insere_lista(v, g->vertices);
		//printf("%s\n", v->jogada);
		i++;
		popula(g, v, ALT_MAX); //vertice, altura máxima
		i = encontra_jogada(r_aux, i); // Procura a partir de i um '.' e devolve a posição dele
	}

}

int existe_chute(vertice v){
	int i;
	for (i = 0; i < v->k; i++) {
		if (v->jogada[i] == 'o') {
			break;
		}
	}
	if ((i == 0 && v->jogada[1] == 'f') || (i == v->k - 1 && v->jogada[v->k - 2] == 'f')) {
		return 1;
	}else{
		if (v->jogada[i + 1] == 'f' || v->jogada[i - 1] == 'f') {
			return 1;
		}else{
			return 0;
		}
	}
}

void popula (grafo g, vertice r_aux, int h){
	if (r_aux->nivel == h && r_aux->gol == 0) { // testa se está na última altura desejada
		r_aux->valor = analisa_jogada(r_aux->jogada, r_aux->k, g->l);
		return;
	}
	int *puloslocal;
	puloslocal = malloc(sizeof(int)*g->k);
	int numpulos = 0;
	vertice v;
	if (existe_chute(r_aux)) {
		// Gera jogadas de chute
		int bolapos;
		for (bolapos = 0; bolapos < r_aux->k; bolapos++) {
			if (r_aux->jogada[bolapos] == 'o') {
				break;
			}
		}
		int indice = bolapos; // verifica posicoes direita
		// verificar direita da bola
		char ultima_jogada = ' ';
		int pode_continuar = 1;
		while ((indice != r_aux->k - 1) && (pode_continuar)){
			if (r_aux->jogada[indice + 1] == 'f'){
				// da pra procurar mais
				ultima_jogada = 'f';
			}
			else if (r_aux->jogada[indice + 1] == '.'){
				if (ultima_jogada == 'f'){
					// gerar jogada
					v = constroi_vertice();
					v->jogada = malloc(sizeof(char)*g->k);
					v->pulos = malloc(sizeof(int)*g->k);
					v->tipojogada = 'o';
					strcpy(v->jogada, r_aux->jogada);
					puloslocal[numpulos] = indice + 2;
					numpulos++;
					v->tamanhopulos = numpulos; // pulos tem que começar em 1
					copia_vetor_int(v->pulos, puloslocal, g->k);
					int limpador;
					for (limpador = bolapos; limpador <= indice; limpador++)
						v->jogada[limpador] = '.'; // limpar a posição
					v->jogada[limpador] = 'o';
					v->k = g->k;
					v->nivel = r_aux->nivel + 1;
					insere_lista (v, r_aux->vizinhos);
					insere_lista(v, g->vertices);
					//printf("	%s\n", v->jogada);
					popula(g, v, ALT_MAX); //vertice, altura máxima
				}
				else{
					pode_continuar = 0;
				}
				ultima_jogada = '.';
			}
			indice++;

		}
		if (pode_continuar && ultima_jogada == 'f'){
			// gerar jogada de gol
			v = constroi_vertice();
			v->jogada = malloc(sizeof(char)*g->k);
			v->k = g->k;
			v->pulos = malloc(sizeof(int)*g->k);
			v->tipojogada = 'o';
			v->gol = 1;
			puloslocal[numpulos] = g->k;
			v->nivel = r_aux->nivel + 1;
			v->valor = ((v->k)*(v->k)*2) - (v->nivel);
			if (g->l == 'd') {
				v->valor = (v->valor)*(-1);
			}
			numpulos++;
			v->tamanhopulos = numpulos; // pulos tem que começar em 1
			copia_vetor_int(v->pulos, puloslocal, g->k);
			strcpy(v->jogada, r_aux->jogada);
			int limpador;
			for (limpador = bolapos; limpador <= indice; limpador++)
				v->jogada[limpador] = '.'; // limpar a posição
			insere_lista (v, r_aux->vizinhos);
			insere_lista(v, g->vertices);
			//printf("	%s\n", v->jogada);
		}
		free(puloslocal);
		puloslocal = malloc(sizeof(int)*g->k);
		numpulos = 0;
		indice = bolapos;

		// verificar esquerda da bola
		ultima_jogada = ' ';
		pode_continuar = 1;
		while ((indice != 0) && (pode_continuar)){
			if (r_aux->jogada[indice - 1] == 'f'){
				// da pra procurar mais
				ultima_jogada = 'f';
			}
			else if (r_aux->jogada[indice - 1] == '.'){
				if (ultima_jogada == 'f'){
					// gerar jogada
					v = constroi_vertice();
					v->jogada = malloc(sizeof(char)*g->k);
					strcpy(v->jogada, r_aux->jogada);
					v->pulos = malloc(sizeof(int)*g->k);
					v->tipojogada = 'o';
					puloslocal[numpulos] = indice;
					numpulos++;
					// for (int w = 0; w < numpulos; w++) {
					// 	printf("%d\n", puloslocal[w]);
					// }
					v->tamanhopulos = numpulos; // pulos tem que começar em 1
					copia_vetor_int(v->pulos, puloslocal, g->k);
					int limpador;
					for (limpador = bolapos; limpador >= indice; limpador--)
						v->jogada[limpador] = '.'; // limpar a posição
					v->jogada[limpador] = 'o';
					v->k = g->k;
					v->nivel = r_aux->nivel + 1;
					insere_lista (v, r_aux->vizinhos);
					//imprime_debug("Existe chute");
					insere_lista(v, g->vertices);
					popula(g, v, ALT_MAX); //vertice, altura máxima
				}
				else{
					pode_continuar = 0;
				}
				ultima_jogada = '.';
			}
			indice--;

		}
		if (pode_continuar && ultima_jogada == 'f'){
			// gerar jogada de gol
			v = constroi_vertice();
			v->jogada = malloc(sizeof(char)*g->k);
			v->pulos = malloc(sizeof(int)*g->k);
			v->k = g->k;
			v->tipojogada = 'o';
			puloslocal[numpulos] = 1;
			v->nivel = r_aux->nivel + 1;
			v->valor = ((v->k)*(v->k)*2) - (v->nivel);
			if (g->l == 'e') {
				v->valor = (v->valor)*(-1);
			}
			numpulos++;
			v->tamanhopulos = numpulos; // pulos tem que começar em 1
			copia_vetor_int(v->pulos, puloslocal, g->k);
			v->gol = 1;
			strcpy(v->jogada, r_aux->jogada);
			int limpador;
			for (limpador = bolapos; limpador >= indice; limpador--)
				v->jogada[limpador] = '.'; // limpar a posição
			insere_lista (v, r_aux->vizinhos);
			insere_lista(v, g->vertices);
		}

	}


	int i = 0;
	i = encontra_jogada(r_aux, i); // Procura a partir de i um '.' e devolve a posição dele
	while(i != -1){
		v = constroi_vertice();
		v->jogada = malloc(sizeof(char)*g->k);
		strcpy(v->jogada, r_aux->jogada);
		v->pulos = malloc(sizeof(int));
		v->tamanhopulos = 1; // pulos tem que começar em 1
		v->pulos[0] = i + 1;
		v->tipojogada = 'f';
		v->jogada[i] = 'f'; //Coloca o jogador já na posição
		v->k = g->k;
		v->nivel = r_aux->nivel + 1;
		insere_lista (v, r_aux->vizinhos);
		insere_lista(v, g->vertices);
		//printf("	%s\n", v->jogada);
		i++;
		popula(g, v, ALT_MAX); //vertice, altura máxima
		i = encontra_jogada(r_aux, i); // Procura a partir de i um '.' e devolve a posição dele
	}
	return;
}

void minmax(grafo g){
	vertice v = g->raiz;
	vertice aux;
	int *pulos;
	pulos = malloc(sizeof(int)*g->k);
	char tipojogada;
	int tamanhopulos;
	int maior;


	no n_lista;
	// for (n_lista = primeiro_no(v->vizinhos); n_lista; n_lista=proximo_no(n_lista)) {
	// 	aux = conteudo(n_lista);
	// 	if (!aux)
	// 		imprime_debug("null");
	// 	printf("jogada %s\n", aux->jogada);
	// 	printf("valor %d tipo %c\n", aux->valor, aux->tipojogada);
	// 	for (int i = 0; i < aux->tamanhopulos; i++) {
	// 		printf("%d ", aux->pulos[i]);
	// 	}
	// 	imprime_debug(" ");
	// }
	n_lista = primeiro_no(v->vizinhos);
	//printf("tam vizinhos v %d\n", tamanho_lista(v->vizinhos));
	aux = conteudo(n_lista);
	//imprime_debug("minmax");
	maior = minmax_recursivo(aux, g->l);

	copia_vetor_int(pulos, aux->pulos, aux->tamanhopulos);
	tamanhopulos = aux->tamanhopulos;
	tipojogada = aux->tipojogada;

	for (n_lista = proximo_no(n_lista); n_lista; n_lista=proximo_no(n_lista)) {
	 	aux = conteudo(n_lista);
		int aux2 = minmax_recursivo(aux, g->l);
		if (g->l == 'd'){
			if (aux2 > maior) {
				maior = aux2;
				copia_vetor_int(pulos, aux->pulos, aux->tamanhopulos);
				tamanhopulos = aux->tamanhopulos;
				tipojogada = aux->tipojogada;
			}
			else {
				free(aux->pulos);
			}
		}
		else {
			if (aux2 >= maior) {
				//printf("aux2 %d\n", aux2);
				maior = aux2;
				copia_vetor_int(pulos, aux->pulos, aux->tamanhopulos);
				// for (int i = 0; i < aux->tamanhopulos; i++) {
				// 	printf(" %d", aux->pulos[i]);
				// }
				//imprime_debug(" ");
				tamanhopulos = aux->tamanhopulos;
				tipojogada = aux->tipojogada;
			}
			else {
				free(aux->pulos);
			}
		}

	}
	//printf("maior %d\n", maior);
		char buf [512];
		int offset =  3;
		sprintf(buf, "%c %c", g->l, tipojogada);
		// printf("%c %c", g->l, tipojogada);
		for (int i = 0; i < tamanhopulos; i++, offset+=2) {
			// printf(" %d", pulos[i]);
				sprintf(&(buf[offset]), " %d", pulos[i]);
			if(pulos[i] > 9)
				offset+=1;
			if(pulos[i] > 99)
				offset+=1;
		}
		 // printf("\n");
		sprintf(&(buf[offset]), "\n");
		printf("%s", buf);
		campo_envia(buf);
		free (pulos);

}

int minmax_recursivo(vertice v, char l){
	if (v->nivel == ALT_MAX) {
		return v->valor;
	}
	else if (v->gol){
		return v->valor;
	}

	vertice aux;

	int maior;
	int menor;
	no n_lista = primeiro_no(v->vizinhos);
	aux = conteudo(n_lista);
	if (aux->nivel % 2 ==0) {//par é min
		menor = minmax_recursivo(aux, l);

		for (n_lista = proximo_no(n_lista); n_lista; n_lista=proximo_no(n_lista)) {
		 	aux = conteudo(n_lista);
			int aux2 = minmax_recursivo(aux, l);
			if (l == 'd'){
				if (aux2 < menor)
					menor = aux2;

			}
			else {
				if (aux2 <= menor)
					menor = aux2;
			}
			free(aux->pulos);
		}
		return menor;
	}
	else{
		maior = minmax_recursivo(aux, l);
		for (n_lista = proximo_no(n_lista); n_lista; n_lista=proximo_no(n_lista)) {
		 	aux = conteudo(n_lista);
			int aux2 = minmax_recursivo(aux, l);
			if (l == 'd'){
				if (aux2 > maior)
					maior = aux2;
			}
			else {
				if (aux2 >= maior)
					maior = aux2;
			}

		}
		return maior;
	}
}



int analisa_jogada (char *jogada, int k, char l){
	int bolapos;
	int meio = (k-1)/2;
	for (bolapos = 0; bolapos < k; bolapos++) { // achar posicao da bola
		if (jogada[bolapos] == 'o') {
			break;
		}
	}
	int *jogadapesos;
	jogadapesos = calloc(k, sizeof(int));
	int i, j;

	int contpeso = 0;
	char ultima_jogada = ' ';
	for (i=0; i < k; i++){ // percorre vetor de jogadas
		if (i==bolapos){ // i é bola
			if (i == 0){ // bola do lado do gol (multiplicadores sao invertidos)
				if (l == 'e'){ // nosso lado é esquerdo
					jogadapesos[i] = k*k;
				}
				else {
					jogadapesos[i] = (-1)*(k*k);
				}
			}
			else if (i == k-1){ // bola do lado do gol (multiplicadores sao invertidos)
				if (l == 'd'){
					jogadapesos[i] = k*k;
				}
				else {
					jogadapesos[i] = (-1)*(k*k);
				}
			}
			else if (i < meio){ // bola no lado esquerdo (ignorando bola no meio)	432101234  meio = 4
				contpeso = meio - i; // 3
				ultima_jogada = ' ';
				for (j = i + 1; j < meio; j++){
					if (j == 'f'){
						contpeso += meio - j; // 5
						ultima_jogada = 'f';
					}
					else if (j == '.'){
						if (ultima_jogada == '.'){
							break;
						}
						else if (ultima_jogada == 'f'){
							ultima_jogada = '.';
						}
					}
				}
				if (l == 'd'){
					jogadapesos[i] = (-1)*(contpeso);
				}
				else {
					jogadapesos[i] = contpeso;
				}
			}
			else if (i > meio){ // bola no lado direito (ignorando bola no meio)
				contpeso = i - meio;
				ultima_jogada = ' ';
				for (j = i - 1; j > meio; j--){
					if (j == 'f'){
						contpeso += j - meio;
						ultima_jogada = 'f';
					}
					else if (j == '.'){
						if (ultima_jogada == '.'){
							break;
						}
						else if (ultima_jogada == 'f'){
							ultima_jogada = '.';
						}
					}
				}
				if (l == 'e'){
					jogadapesos[i] = contpeso;
				}
				else {
					jogadapesos[i] = (-1)*(contpeso);
				}
			}
		}
		else {
			// i nao é bola
			if (jogada[i] == 'f' && jogadapesos[i] == 0){ // posicao e jogador e nao esta relacionado com a bola
				if (i < meio){
					if (l == 'd'){
						jogadapesos[i] = meio - i;
					}
					else {
						jogadapesos[i] = (-1)*(meio - i);
					}
				}
				else if (i > meio){
					if (l == 'e'){
						jogadapesos[i] = i - meio;
					}
					else {
						jogadapesos[i] = (-1)*(i - meio);
					}
				}
			}
		}
	}

	int soma = 0;
	for (i = 0; i < k; i++)
		soma += jogadapesos[i];

	free(jogadapesos);
	//printf("%s\n %d\n", jogada, soma);
	return soma;

}

int encontra_jogada(vertice v, int i){
	for (int j = i; j < v->k; j++) {
		//printf("jogada %c\n", v->jogada[j]);
		if (v->jogada[j] == '.') {
			return j;
		}
	}
	return -1;
}

void imprime_lista(lista l){
	int *novo;
	for (no n_lista = primeiro_no(l); n_lista; n_lista=proximo_no(n_lista)) {
	 	novo = conteudo(n_lista);
		printf("%d ", *novo);
	}
	printf("\n");
	return;
}

void imprime_debug(char *c){
	#ifndef DEBUG
	printf("%s\n", c);
	#endif
}



//------------------------------------------------------------------------------
// Funcao auxiliar usada por destroi_grafo() para eliminar o conteudo de um vertice na lista
// de vertices
// devolve 1 em caso de sucesso ou
//         0 caso contrário
// static int destroi_indice(void *p) {
// 	if (!p)
// 		return 0;
// 	free(p);
// 	p = NULL;
// 	return 1;
// }

//------------------------------------------------------------------------------
// Funcao auxiliar usada por destroi_grafo() para eliminar o conteudo de um vertice na lista
// de vertices
// devolve 1 em caso de sucesso ou
//         0 caso contrário
static int destroi_vertice(void *p) {
	vertice v = p;
    if (v) {
		no n;
		if (destroi_lista(v->vizinhos, NULL)){
			v->vizinhos = NULL;
			free(v->jogada);


			free (v);
			v = NULL;
			return 1;
		}
    } else {
        return 0;
    }
}



//------------------------------------------------------------------------------
// Insere nas listas de adjacências dos vértices envolvidos a alteração na vizinhança.
// No caso do direcionado, o vértice head, tem em sua vizinhança quem aponta pra ele
// No caso não direcionado, é adicionado na lista do head que ele tambem aponta para o tail.
// Os if's com existe_vertice() é para evitar que se existe duas arestas iguais
// 		Não haver o mesmo vértice duplicado na lista de vizinhos.
// static int adiciona_vizinhanca (vertice In, vertice Out, grafo g){
// 	if(!existe_vertice(In->vizinhos, Out))
// 		insere_lista(Out, In->vizinhos);
// 	if(!existe_vertice(Out->vizinhos, In))
// 		insere_lista(In, Out->vizinhos);
// 	return 1;
// }

// void set_parametro(lista l, int parametro, int valor){ // parametro 0 => visitado.  1 => comunidade. 2 => indice.
// 	vertice v;
// 	no aux;
// 	if (parametro == 0){
// 		for (aux = primeiro_no(l); aux; aux = proximo_no(aux)) {
// 			v = conteudo(aux);
// 			v->visitado = valor;
// 		}
// 	} else if (parametro == 1){
// 		for (aux = primeiro_no(l); aux; aux = proximo_no(aux)) {
// 			v = conteudo(aux);
// 			v->comunidade = valor;
// 		}
// 	} else if (parametro == 2){
// 		for (aux = primeiro_no(l); aux; aux = proximo_no(aux)) {
// 			v = conteudo(aux);
// 			v->indice = valor;
// 		}
// 	}
// }

grafo gera_grafo (int k, char l, char *campo){
  grafo g = malloc(sizeof(struct grafo));
	if (!g)
		return NULL;
	g->k = k;
	ALT_MAX = 4;
	g->l = l;
	g->vertices = constroi_lista();
	vertice v = constroi_vertice();
	v->jogada = malloc(sizeof(char)*k);
	strcpy(v->jogada, campo);

	// char a[100];
	// scanf("%s", v->jogada);
	// getchar();
	// fgets(a, 100, stdin);
	// printf("%s", v->jogada);
	v->k = k;
	v->nivel = 1;
	g->raiz = v;
	insere_lista(v, g->vertices);
	return g;
}

// //------------------------------------------------------------------------------
// // desaloca toda a memória usada em *g
// //(grafo)io
// //
// // g é um (void *) para que destroi_grafo() possa ser usada como argumento de
// // destroi_lista()
int destroi_grafo(void *g) {
    grafo novo_g = g;
    no n;
    if (novo_g) {
      if ((destroi_lista(novo_g->vertices, destroi_vertice))) {
				free(g);
      	return 1;
      }
    }
    return 0;
}

// //------------------------------------------------------------------------------
// // devolve um grafo igual a g

// grafo copia_grafo(grafo g){
// 	grafo g_copy = malloc(sizeof(struct grafo));
// 	vertice v;
// 	vertice temp_v;
//
// 	g_copy->vertices = constroi_lista();
// 	// DADOS DA STRUCT GRAFO INTEIRO
// 	g_copy->k = g->k;
//
// 	no v_entrada;
// 	// Gera copias de TODOS os vértices
// 	for (v_entrada = primeiro_no(g->vertices); v_entrada; v_entrada = proximo_no(v_entrada)) {
// 		v = constroi_vertice();
// 		temp_v = conteudo(v_entrada);
// 		v->visitado = temp_v->visitado;
// 		v->l = temp_v->l;
// 		v->c = temp_v->c;
// 		v->cor = temp_v->cor;
// 		v->comunidade = temp_v->comunidade;
// 		v->peso = temp_v->peso;
// 		v->indice = temp_v->indice;
// 		insere_lista (v, g_copy->vertices);
// 	}
// 	no vizinho;
// 	vertice temp_v2;
// 	vertice v2;
// 	// Após já ter todos os vértices copiados, gera a lista de vizinhos
// 	for (v_entrada = primeiro_no(g->vertices); v_entrada; v_entrada = proximo_no(v_entrada)) {
// 		temp_v = conteudo(v_entrada);
// 		v = busca_vertice_mapa(g_copy->vertices, temp_v->l, temp_v->c); // 'v' é o Vertice equivalente no g_copy
// 		for (vizinho = primeiro_no(temp_v->vizinhos); vizinho; vizinho = proximo_no(vizinho)) {
// 			temp_v2 = conteudo(vizinho);
// 			v2 = busca_vertice_mapa(g_copy->vertices, temp_v2->l, temp_v2->c);
// 			insere_lista(v2, v->vizinhos);
// 		}
// 	}
// 	return g_copy;
// }
