#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"

struct vertice
{
	char *jogada;
	int nivel;
	int valor;
	int k;
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

void popula_grafo (grafo g){
	vertice r_aux = g->raiz;
	if (existe_chute(r_aux)) {
		// Gera jogadas de chute
	}
	int i = 0;
	vertice v;
	while(i != -1){
		i = encontra_jogada(r_aux, i); // Procura a partir de i um '.' e devolve a posição dele
		v = constroi_vertice();
		v->jogada = malloc(sizeof(char)*g->k);
		strcpy(v->jogada, r_aux->jogada);
		v->jogada[i] = 'f'; //Coloca o jogador já na posição
		v->k = g->k;
		v->nivel = 2;
		insere_lista (v, r_aux->vizinhos);
		insere_lista(v, g->vertices);
		i++;
		popula(g, v, 3); //vertice, altura máxima
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

void popula (grafo g, vertice v, int h){
	if (v->nivel == h) { // testa se está na última altura desejada
		v->valor = analisa_jogada(v->jogada, v->k);
		return;
	}
	if (existe_chute(v)) {
		// Gera jogadas de chute
	}
	int i = 0;
	vertice aux;
	while(i != -1){
		i = encontra_jogada(v, i); // Procura a partir de i um '.' e devolve a posição dele
		aux = constroi_vertice();
		aux->jogada = malloc(sizeof(char)*g->k);
		strcpy(aux->jogada, v->jogada);
		aux->jogada[i] = 'f'; //Coloca o jogador já na posição
		aux->k = g->k;
		aux->nivel = v->nivel + 1;
		insere_lista (aux, r_aux->vizinhos);
		insere_lista(aux, g->vertices);
		i++;
		popula(g, aux, 3); //vertice, altura máxima
	}
	return;
}

int encontra_jogada(vertice v, int i){
	for (int j = i; j < v->k; j++) {
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
//Aloca memoria para um vertice v
static vertice constroi_vertice(void) {
	vertice v = malloc(sizeof(struct vertice));
	if (!v)
		return NULL;
	v->nivel = 0;
	v->valor = 0;
	v->k = 0;
	v->vizinhos = constroi_lista();
	return v;
}


//------------------------------------------------------------------------------
// Busca um vertice v na lista dada
static vertice busca_vertice(lista l, vertice v){
	vertice novo;
	for (no n_lista = primeiro_no(l); n_lista; n_lista=proximo_no(n_lista)) {
	 	novo =  conteudo(n_lista);
		if (v->l == novo->l && v->c == novo->c)
			return novo;
	 }
	 return NULL;
}

//------------------------------------------------------------------------------
// Verifica se vertice esta na lista l
static int existe_vertice(lista l, vertice v){
	vertice novo;
	for (no n_lista = primeiro_no(l); n_lista; n_lista=proximo_no(n_lista)) {
	 	novo =  conteudo(n_lista);
		if (v->l == novo->l && v->c == novo->c)
			return 1;
	 }
	 return 0;
}



//------------------------------------------------------------------------------
// Funcao auxiliar usada por destroi_grafo() para eliminar o conteudo de um vertice na lista
// de vertices
// devolve 1 em caso de sucesso ou
//         0 caso contrário
static int destroi_indice(void *p) {
	if (!p)
		return 0;
	free(p);
	p = NULL;
	return 1;
}

//------------------------------------------------------------------------------
// Funcao auxiliar usada por destroi_grafo() para eliminar o conteudo de um vertice na lista
// de vertices
// devolve 1 em caso de sucesso ou
//         0 caso contrário
static int destroi_vertice(void *p) {
	vertice v = p;
    if (v) {
		no n;
		if ((destroi_lista(v->vizinhos, NULL) && (destroi_lista(v->passos, NULL)))){
			v->vizinhos = NULL;
			v->passos = NULL;
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
static int adiciona_vizinhanca (vertice In, vertice Out, grafo g){
	if(!existe_vertice(In->vizinhos, Out))
		insere_lista(Out, In->vizinhos);
	if(!existe_vertice(Out->vizinhos, In))
		insere_lista(In, Out->vizinhos);
	return 1;
}

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


int existe_vizinhanca (vertice original, int indice){
	no aux2;
	vertice v_aux2;
	for (aux2 = primeiro_no(original->vizinhos); aux2; aux2 = proximo_no(aux2)) {
		v_aux2 = conteudo(aux2);
		if (v_aux2->indice == indice)
			return 1;
	}
	return 0;
}

grafo gera_grafo (int k, char l){
  grafo g = malloc(sizeof(struct grafo));
	if (!g)
		return NULL;
	g->k = k;
	g->l = l;
	g->vertices = constroi_lista();
	vertice v = constroi_vertice();
	v->jogada = malloc(sizeof(char)*k);
  scanf("%s", v->jogada);
	v->k = k;
	v->nivel = 1;
	g->raiz = v;
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
