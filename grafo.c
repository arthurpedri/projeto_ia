#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"

struct vertice
{
	int visitado;
	int l;
	int c;
	int cor;
	lista vizinhos;
};

struct grafo
{
	int nl;
	int nc;
	int cores;
	lista vertices;
};

unsigned int n_vertices(grafo g){
	return tamanho_lista(g->vertices);
}


//------------------------------------------------------------------------------
//Aloca memoria para um vertice v
static vertice constroi_vertice(void) {
	vertice v = malloc(sizeof(struct vertice));
	if (!v)
		return NULL;
	v->visitado = 0;
	v->l = 0;
	v->c = 0;
	v->indice = 0;
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

static vertice busca_vertice_mapa(lista l, int l, int c){
	vertice novo;
	for (no n_lista = primeiro_no(l); n_lista; n_lista=proximo_no(n_lista)) {
	 	novo =  conteudo(n_lista);
		if (l == novo->l && c == novo->c)
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
//------------------------------------------------------------------------------
// lê um grafo no formato dot de input, usando as rotinas de libcgraph
//
// desconsidera todos os atributos do grafo lido exceto o atributo
// "peso" quando ocorrer; neste caso o valor do atributo é o peso da
// aresta/arco que é um long int
//
// num grafo com pesos todas as arestas/arcos tem peso
//
// o peso default de uma aresta num grafo com pesos é 0
//
// todas as estruturas de dados alocadas pela libcgraph são
// desalocadas ao final da execução
//
// devolve o grafo lido ou
//         NULL em caso de erro
grafo le_grafo(tmapa *m){

	if (!m)
		return NULL;

	grafo g = malloc(sizeof(struct grafo));
	if (!g)
		return NULL;

	g->nc = m->ncolunas;
	g->nl = m->nlinhas;
	g->cores = m->ncores;
	g->vertices = constroi_lista();
	vertice v;

	// Percorre todas as posições da matriz para criar todos os vértices
	for (int i = 0; i < g->nl; i++;) {
		for (int j = 0; j < g->nc; j++) {
			v = constroi_vertice();
			v->l = i;
			v->c = j;
			v->cor = m->mapa[i][j];
			insere_lista (v, g->vertices);
		}
	}

	vertice aux;
	// Gera a lista de adijacência para cada vértice, percorrendo a matriz

	//Cantos
	v = busca_vertice_mapa(g->vertices, 0, 0);
	aux = busca_vertice_mapa(g->vertices, 0, 1);
	insere_lista(aux, v->vizinhos);
	aux = busca_vertice_mapa(g->vertices, 1, 0);
	insere_lista(aux, v->vizinhos);

	v = busca_vertice_mapa(g->vertices, 0, g->nc - 1);
	aux = busca_vertice_mapa(g->vertices, 0, g->nc - 2);
	insere_lista(aux, v->vizinhos);
	aux = busca_vertice_mapa(g->vertices, 1, g->nc - 1);
	insere_lista(aux, v->vizinhos);

	v = busca_vertice_mapa(g->vertices, g->nl-1, 0);
	aux = busca_vertice_mapa(g->vertices, g->nl - 1, 1);
	insere_lista(aux, v->vizinhos);
	aux = busca_vertice_mapa(g->vertices, g->nl - 2, 0);
	insere_lista(aux, v->vizinhos);

	v = busca_vertice_mapa(g->vertices, g->nl - 1, g->nc - 1);
	aux = busca_vertice_mapa(g->vertices, g->nl - 2, g->nc - 1);
	insere_lista(aux, v->vizinhos);
	aux = busca_vertice_mapa(g->vertices, g->nl - 1, g->nc - 2);
	insere_lista(aux, v->vizinhos);

	//Linhas horizontais, sem as posições do canto
	for (int j = 1; j < g->nc - 1; j++) {
		v = busca_vertice_mapa(g->vertices, 0, j);
		aux = busca_vertice_mapa(g->vertices, 0, j-1);
		insere_lista(aux, v->vizinhos);
		aux = busca_vertice_mapa(g->vertices, 0, j+1);
		insere_lista(aux, v->vizinhos);
		aux = busca_vertice_mapa(g->vertices, 1, j);
		insere_lista(aux, v->vizinhos);

		v = busca_vertice_mapa(g->vertices, g->nl - 1, j);
		aux = busca_vertice_mapa(g->vertices, g->nl - 1, j-1);
		insere_lista(aux, v->vizinhos);
		aux = busca_vertice_mapa(g->vertices, g->nl - 1, j+1);
		insere_lista(aux, v->vizinhos);
		aux = busca_vertice_mapa(g->vertices, g->nl - 2, j);
		insere_lista(aux, v->vizinhos);
	}
	//Linhas verticais, sem as posições do canto
	for (int i = 1; i < g->nl - 1; i++) {
		v = busca_vertice_mapa(g->vertices, i, 0);
		aux = busca_vertice_mapa(g->vertices, i-1, 0);
		insere_lista(aux, v->vizinhos);
		aux = busca_vertice_mapa(g->vertices, i, 1);
		insere_lista(aux, v->vizinhos);
		aux = busca_vertice_mapa(g->vertices, i+1, 0);
		insere_lista(aux, v->vizinhos);

		v = busca_vertice_mapa(g->vertices, i, g->nc - 1);
		aux = busca_vertice_mapa(g->vertices, i-1, g->nc - 1);
		insere_lista(aux, v->vizinhos);
		aux = busca_vertice_mapa(g->vertices, i, g->nc - 2);
		insere_lista(aux, v->vizinhos);
		aux = busca_vertice_mapa(g->vertices, i+1, g->nc - 1);
		insere_lista(aux, v->vizinhos);
	}
	// Percorre o miolo (1 1 até nl-1 nc-1)
	for (int i = 1; i < g->l - 1; i++) {
		for (int j = 1; j < g->c - 1; i++) {
			v = busca_vertice_mapa(g->vertices, i, j);

			aux = busca_vertice_mapa(g->vertices, i-1, j);
			insere_lista(aux, v->vizinhos);
			aux = busca_vertice_mapa(g->vertices, i, j+1);
			insere_lista(aux, v->vizinhos);
			aux = busca_vertice_mapa(g->vertices, i+1, j);
			insere_lista(aux, v->vizinhos);
			aux = busca_vertice_mapa(g->vertices, i, j-1);
			insere_lista(aux, v->vizinhos);
		}
	}

	return g;

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
		if ((destroi_lista(v->vizinhos, NULL))){
			v->vizinhos = NULL;
			free (v);
			v = NULL;
			return 1;
		}
    } else {
        return 0;
    }
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
grafo copia_grafo(grafo g){
	grafo g_copy = malloc(sizeof(struct grafo));
	vertice v;
	vertice temp_v;

	g_copy->vertices = constroi_lista();

	no v_entrada;
	// Gera copias de TODOS os vértices
	for (v_entrada = primeiro_no(g->vertices); v_entrada; v_entrada = proximo_no(v_entrada)) {
		v = constroi_vertice();
		temp_v = conteudo(v_entrada);
		v->visitado = temp_v->visitado;
		v->l = temp_v->l;
		v->c = temp_v->c;
		v->indice = temp_v->indice;
		insere_lista (v, g_copy->vertices);
	}
	no vizinho;
	vertice temp_v2;
	vertice v2;
	// Após já ter todos os vértices copiados, gera a lista de vizinhos
	for (v_entrada = primeiro_no(g->vertices); v_entrada; v_entrada = proximo_no(v_entrada)) {
		temp_v = conteudo(v_entrada);
		v = busca_vertice_mapa(g_copy->vertices, temp_v->l, temp_v->c); // 'v' é o Vertice equivalente no g_copy
		for (vizinho = primeiro_no(temp_v->vizinhos); vizinho; vizinho = proximo_no(vizinho)) {
			temp_v2 = conteudo(vizinho);
			v2 = busca_vertice(g_copy->vertices, temp_v2->l, temp_v2->c);
			insere_lista(v2, v->vizinhos);
		}
	}
	return g_copy;
}

// //------------------------------------------------------------------------------
// // devolve a vizinhança do vértice v no grafo g
// //
lista vizinhanca(vertice v, grafo g) {
	if (existe_vertice(g->vertices, v)){
		return v->vizinhos;
	}
}

// //------------------------------------------------------------------------------
// // devolve o grau do vértice v no grafo g
// //
unsigned int grau(vertice v, grafo g)
{
	if (existe_vertice(g->vertices, v)){
		return tamanho_lista(v->vizinhos);
	}
}

// //------------------------------------------------------------------------------
// // devolve 1, se o conjunto dos vertices em l é uma clique em g, ou
// //         0, caso contrário
// //
// // um conjunto C de vértices de um grafo é uma clique em g
// // se todo vértice em C é vizinho de todos os outros vértices de C em g
int clique(lista l, grafo g) {
	unsigned int tam = tamanho_lista(l);
	if (tam == 0) //Se vizinhanca eh vazia, eh clique
		return 1;

	vertice v;
	lista vizinhos_v;

	for(no n = primeiro_no(l); n && proximo_no(n) != NULL; n = proximo_no(n)) {
		v = conteudo(n);
		vertice pv;
		vizinhos_v = vizinhanca(v, g);

		//Se o grau do vertice != tam - 1 nao eh vizinho de todos ou tem mais vizinhos
		if (grau(v, g) < (tam - 1)) {
			return 0;
		//Se o grau esta correto, precisa vericar vizinhanca
		} else {
			for (no prox = proximo_no(n); prox; prox = proximo_no(prox)) {
				pv = conteudo(prox);
				if(!existe_vertice(vizinhos_v, conteudo(prox))){
					return 0;
				}
			}
		}
	}
	return 1;
}

// //------------------------------------------------------------------------------
// // devolve 1, se v é um vértice simplicial em g, ou
// //         0, caso contrário
// //
// // um vértice é simplicial no grafo se sua vizinhança é uma clique
int simplicial(vertice v, grafo g) {
	if (existe_vertice(g->vertices, v)) {
		if(clique(vizinhanca(v, g), g))
			return 1;
	}
	return 0;
}


void insere_label (vertice v, int i){
	v->label[v->indice] = i;
	v->indice++;
}

//------------------------------------------------------------------------------
// devolve o vertice de maior label lexicografico
static vertice maior_label(lista l){
	vertice maior = conteudo(primeiro_no(l));
	vertice v_temp;
	int tam = tamanho_lista(l);
	no n = primeiro_no(l); // O primeiro é sempre igual ao maior
	n = proximo_no(n); // Começa o 'for' pelo segundo nó da lista
	for (n; n; n = proximo_no(n)) {
		v_temp = conteudo(n);
		if (v_temp->visitado == 0 ){
			int i = 0;
			while (v_temp->label[i] == maior->label[i] && i < tam && v_temp->label[i] != 0 && maior->label[i] != 0) {
				i++;
				if (v_temp->label[i] < maior->label[i])
					break;
			}
			if (v_temp->label[i] > maior->label[i]) {
				maior = v_temp;
			} else {
				return v_temp;
			}
		}
	}
	return maior;
}

//------------------------------------------------------------------------------
// devolve uma lista de vertices com a ordem dos vértices dada por uma
// busca em largura lexicográfica

lista busca_largura_lexicografica(grafo g){
	//Algoritmo
	//setar label = 0 para todos os vertices
	//label(primeiro_no) = n (|V(G)|)
	//for i = n até 1
	//	escolher vertice nao visitado v com maior label lexicografico
	//	resultado[i] = v
	//	foreach vizinho de v nao visitado w  do
	//		label(w) = label(w).{i}
	//	end
	//end
	lista ordem = constroi_lista();
	int tam = tamanho_lista(g->vertices);
	vertice v_temp;
	for (no n = primeiro_no(g->vertices); n; n = proximo_no(n)) {
		v_temp = conteudo(n);
		v_temp->label = calloc(tam, sizeof(int));
	}
	vertice v = conteudo(primeiro_no(g->vertices));
	insere_label(v, tam);

	for (int i = tam ; i >= 1; i--) {
		vertice maior = maior_label(g->vertices);
		maior->visitado = 1;
		insere_lista(maior, ordem);
		for (no m = primeiro_no(maior->vizinhos); m; m = proximo_no(m)) {
			v_temp = conteudo(m);
			if (v_temp->visitado == 0) {
				insere_label(v_temp, i);
			}
		}

	}

	return ordem;
}

//------------------------------------------------------------------------------
// devolve 1, se a lista l representa uma
//            ordem perfeita de eliminação para o grafo g ou
//         0, caso contrário
//
// o tempo de execução é O(|V(G)|+|E(G)|)

int ordem_perfeita_eliminacao(lista l, grafo g){
	vertice v, w, v_temp;
	no n;
	int tam_vizinhos_v, cont, i;

	//reseta os vertices para nao visualizados
	for (n = primeiro_no(l); n; n = proximo_no(n)){
		v_temp = conteudo(n);
		v_temp->visitado = 0;
	}
	//percorre a lista de ordem de eliminacao
	for (i = 0, n = primeiro_no(l); n; n = proximo_no(n), ++i){
		cont = tam_vizinhos_v = 0;
		v = conteudo(n);

		//marca os vizinhos de v
		for (no aux1 = primeiro_no(v->vizinhos); aux1; aux1 = proximo_no(aux1)) {
			v_temp = conteudo(aux1);
			v_temp->vizinho = i; //Marca v_temp como vizinho de v
			if (v_temp->visitado == 0){
				tam_vizinhos_v++;
			}
		}

		//encontra primeiro vizinho de v
		for (no aux2 = proximo_no(n); aux2; aux2 = proximo_no(aux2)) {
			v_temp = conteudo(aux2);
			if(v_temp->vizinho == i) {
				w = v_temp;
				break;
			}
		}

		//verifica se os vizinhos de w sao os vizinhos de v
		for (no aux3 = primeiro_no(w->vizinhos); aux3; aux3 = proximo_no(aux3)){
			v_temp = conteudo(aux3);
			if(v_temp->vizinho == i){
				cont++;
			}
		}

		//Se a quantidade de vizinhos de w < que vizinhos de v -1,
		//significa que v possui w e u como vizinhos, e u e w nao sao vizinhos entre si
		if (cont < tam_vizinhos_v -1){
			return 0;
		}
		i++;
		v->visitado = 1;
	}
	return 1;
}
//------------------------------------------------------------------------------
// devolve 1, se g é um grafo cordal ou
//         0, caso contrário

int cordal(grafo g){
	lista ordem = constroi_lista();
	ordem = busca_largura_lexicografica(g);
	int a = ordem_perfeita_eliminacao(ordem, g);
	destroi_lista(ordem, NULL);
	return a;
}
