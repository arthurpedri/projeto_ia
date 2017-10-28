#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"
#define MAX_PRIORI 50

struct vertice
{
	int visitado;
	int l;
	int c;
	int cor;
	int indice;
	int comunidade;
	int peso;
	lista passos;
	lista vizinhos;
};

struct nobusca
{
	int peso;
	int hn;
	int g;
	lista passos;
	lista filhos;
	grafo grafomapa;
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

void imprime_grafo(grafo g){
	vertice v, v2;
	for(no n = primeiro_no(g->vertices); n; n = proximo_no(n)){
		v = conteudo(n);
		printf("%d - [%d,%d] -> %d\n", v->cor, v->l, v->c, v->indice);
		for(no n2 = primeiro_no(v->vizinhos); n2; n2 = proximo_no(n2)){
			v2 = conteudo(n2);
			printf("	%d - [%d,%d]\n", v2->cor, v2->l, v2->c);
		}
		
	}
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
	v->visitado = 0;
	v->l = 0;
	v->c = 0;
	v->indice = 0;
	v->comunidade = 0;
	v->peso = 0;
	v->vizinhos = constroi_lista();
	v->passos = constroi_lista();
	return v;
}

static nobusca constroi_nobusca(void) {
	nobusca v = malloc(sizeof(struct nobusca));
	if (!v)
		return NULL;
	v->peso = 0;
	v->g = 0;
	v->hn = 0;
	v->passos = constroi_lista();
	v->filhos = constroi_lista();
	v->grafomapa = NULL;
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

static vertice busca_indice(lista l, vertice v){
	vertice novo;
	for (no n_lista = primeiro_no(l); n_lista; n_lista=proximo_no(n_lista)) {
	 	novo =  conteudo(n_lista);
		if (v->indice == novo->indice)
			return novo;
	 }
	 return NULL;
}

static int busca_indice_cor(lista l, int indice){
	vertice novo;
	for (no n_lista = primeiro_no(l); n_lista; n_lista=proximo_no(n_lista)) {
	 	novo =  conteudo(n_lista);
		if (indice == novo->indice)
			return novo->cor;
	 }
	 return -1;
}

static vertice busca_vertice_mapa(lista li, int l, int c){
	vertice novo;
	for (no n_lista = primeiro_no(li); n_lista; n_lista=proximo_no(n_lista)) {
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
// Verifica se indice? esta na lista l
static int existe_vertice_indice(lista l, vertice v){
	vertice novo;
	for (no n_lista = primeiro_no(l); n_lista; n_lista=proximo_no(n_lista)) {
	 	novo =  conteudo(n_lista);
		if (v->indice == novo->indice)
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

static int destroi_nobusca(void *p) {
	nobusca v = p;
    if (v) {
		no n;
		if ((destroi_lista(v->passos, NULL)) && (destroi_lista(v->filhos, NULL)) && destroi_grafo(v->grafomapa)){
			v->passos = NULL;
			v->filhos = NULL;
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
	for (int i = 0; i < g->nl; i++) {
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
	
	
	// Percorre o miolo (1 1 até nl-2 nc-2)
	for (int i = 1; i < g->nl - 1; i++) {
		for (int j = 1; j < g->nc - 1; j++) {
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

void set_parametro(lista l, int parametro, int valor){ // parametro 0 => visitado.  1 => comunidade. 2 => indice.
	vertice v;
	no aux;
	if (parametro == 0){
		for (aux = primeiro_no(l); aux; aux = proximo_no(aux)) {
			v = conteudo(aux);
			v->visitado = valor;
		}
	} else if (parametro == 1){
		for (aux = primeiro_no(l); aux; aux = proximo_no(aux)) {
			v = conteudo(aux);
			v->comunidade = valor;
		}
	} else if (parametro == 2){
		for (aux = primeiro_no(l); aux; aux = proximo_no(aux)) {
			v = conteudo(aux);
			v->indice = valor;
		}
	}
}

void gera_indice(vertice v, int *ind_cont, vertice pai){

	vertice filho;
	no aux;
	if (v->comunidade == 1){
		for (aux = primeiro_no(v->vizinhos); aux; aux = proximo_no(aux)) {
			filho = conteudo(aux);
			if (filho->visitado == 0){
				filho->visitado = 1;
				gera_indice(filho, ind_cont, v);
			}
		}
	}
	else {
		v->comunidade = 1;
		v->indice = *ind_cont;
		(*ind_cont)++;
		for (aux = primeiro_no(v->vizinhos); aux; aux = proximo_no(aux)) {
			filho = conteudo(aux);
			if (filho->cor == v->cor){
				filho->comunidade = 1;
				filho->indice = v->indice;
				busca_comunidade(filho);
			}
		}
		for (aux = primeiro_no(v->vizinhos); aux; aux = proximo_no(aux)) {
			filho = conteudo(aux);
			if (filho->visitado == 0){
				filho->visitado = 1;
				gera_indice(filho, ind_cont, v);
			}
		}
	}
}


void busca_comunidade(vertice v){
	vertice filho;
	no aux;
	for (aux = primeiro_no(v->vizinhos); aux; aux = proximo_no(aux)) {
		filho = conteudo(aux);
		if(!filho->comunidade && filho->cor == v->cor){ // se o filho nao tem comunidade e tem a mesma cor do pai
			filho->comunidade = 1;
			filho->indice = v->indice;
			busca_comunidade(filho);
		}
	}
}


int existe_indice(lista l, int indice){
	no aux;
	int *ind;
	if (!tamanho_lista(l) || !l)
		return 0;
	for (aux = primeiro_no(l); aux; aux = proximo_no(aux)) {
		ind = conteudo(aux);
		if (indice == *ind)
			return 1;
	}
	return 0;
}


lista cria_componentes(grafo g) {
	no aux = primeiro_no(g->vertices);
	vertice raiz = conteudo(aux);
	if (raiz->l != 0 && raiz->c != 0)
	{
		return NULL;
	}
	// geração dos indices
	set_parametro(g->vertices, 0, 0); // reseta todos os vertices para nao visitados
	set_parametro(g->vertices, 1, 0); // reseta todos os vertices para nao comunidade
	set_parametro(g->vertices, 2, 0); // reseta todos os vertices para indice 0

	
	vertice filho;
	int *ind_cont;
	ind_cont = malloc(sizeof(int));
	*ind_cont = 2;
	raiz->indice = 1;
	raiz->comunidade = 1;
	raiz->visitado = 1;


	for (aux = primeiro_no(raiz->vizinhos); aux; aux = proximo_no(aux)) {
		filho = conteudo(aux);
		if (filho->cor == raiz->cor){
			filho->comunidade = 1;
			filho->indice = raiz->indice;
			busca_comunidade(filho);
		}
	}
	for (aux = primeiro_no(raiz->vizinhos); aux; aux = proximo_no(aux)) {
		filho = conteudo(aux);
		filho->visitado = 1;
		gera_indice(filho, ind_cont, raiz);
	}
	

	free(ind_cont);
	// criar lista de indice;
	int *ind_aux;
	int tanalista;
	lista indices = constroi_lista();
	lista componentes = constroi_lista();
	vertice componente, v_aux, v_aux2, v_aux3, v_aux4;
	no aux2, aux3;
	
	// cria lista de indices
	for(aux = primeiro_no(g->vertices); aux; aux = proximo_no(aux)){
		v_aux = conteudo(aux);
		if (!existe_indice(indices, v_aux->indice)) { // se nao existe o indice na lista ele adiciona (so precisa de 1 representante do indice na lista)
			ind_aux = malloc(sizeof(int));
			*ind_aux = v_aux->indice;
			insere_lista(ind_aux, indices);
		}
	}
	
	for(aux = primeiro_no(indices); aux; aux = proximo_no(aux)){
		ind_aux = conteudo(aux);
		v_aux = constroi_vertice();
		v_aux->indice = *ind_aux;
		insere_lista(v_aux, componentes);
	}
	
	for(aux = primeiro_no(componentes); aux; aux = proximo_no(aux)){
		v_aux = conteudo(aux);
		for(aux2 = primeiro_no(g->vertices); aux2; aux2 = proximo_no(aux2)){
			v_aux2 = conteudo(aux2);
			if(v_aux2->indice == v_aux->indice){
				for(aux3 = primeiro_no(v_aux2->vizinhos); aux3; aux3 = proximo_no(aux3)){
					v_aux3 = conteudo(aux3);
					if(v_aux2->indice != v_aux3->indice){
						if(!existe_vertice_indice(v_aux->vizinhos, v_aux3)){
							v_aux4 = busca_indice(componentes, v_aux3); // buscar o vertice equivalente na lista de componentes
	 						insere_lista(v_aux4, v_aux->vizinhos);
							if(!existe_vertice_indice(v_aux4->vizinhos, v_aux))
								insere_lista(v_aux, v_aux4->vizinhos);
						}
					}
				}	
			}
		}
	}
	
	
	
	
	for(aux = primeiro_no(componentes); aux; aux = proximo_no(aux)){
		v_aux = conteudo(aux);
		
	}
	
	destroi_lista(indices, NULL);

	set_parametro(g->vertices, 0, 0); // reseta todos os vertices para nao visitados
	return componentes;

}

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

//h(n);
lista h_simples (lista componentes, grafo g){
	no aux, aux2, aux3;
	vertice v, v2, v_aux, v_aux2, v_aux3;
	
	if (tamanho_lista(componentes) == 1){
		return 0;
	}
	set_parametro(componentes, 0, 0);
	int *passo;
	aux = primeiro_no(componentes);
	v_aux = conteudo(aux);
	v_aux->visitado = 1;
	v_aux->peso = 0;
	lista prioridades = constroi_lista();

	for (aux2 = primeiro_no(v_aux->vizinhos); aux2; aux2 = proximo_no(aux2)) {
		v_aux2 = conteudo(aux2);
		v_aux2->visitado = 1;
		passo = malloc(sizeof(int));
 		*passo = busca_indice_cor(g->vertices, v_aux2->indice);
		insere_lista(passo, v_aux2->passos);
 		insere_lista(v_aux2, prioridades);
		v_aux2->peso = v_aux->peso +1;
	}

	while (tamanho_lista(prioridades)){
		aux = primeiro_no(prioridades);
		v_aux = conteudo(aux);
		for (aux2 = primeiro_no(v_aux->vizinhos); aux2; aux2 = proximo_no(aux2)) {
			v_aux2 = conteudo(aux2);
			if (!v_aux2->visitado) {
				v_aux2->visitado = 1;
				v_aux2->peso = v_aux->peso + 1;
				copia_lista_cor(v_aux->passos, v_aux2->passos);
				passo = malloc(sizeof(int));
 				*passo = busca_indice_cor(g->vertices, v_aux2->indice);
				insere_lista(passo, v_aux2->passos);
				insere_lista(v_aux2, prioridades);
				
			}
		}
		remove_no_dry(prioridades, aux, NULL);
	}
	int maior = 0;
	lista resultado = constroi_lista();
	for (aux = primeiro_no(componentes); aux; aux = proximo_no(aux)) {
		v_aux = conteudo(aux);
		if (v_aux->peso > maior) {
			maior = v_aux->peso;
			aux2 = aux;
		}
	}
	
	for (aux = primeiro_no(componentes); aux; aux = proximo_no(aux)) {
		v_aux = conteudo(aux);
		if (aux2 == aux) {
			copia_lista_cor(v_aux->passos, resultado);
		}
	}
	free(prioridades);
	return resultado;
}

//h(n);
int h (lista componentes){
	no aux, aux2;
	vertice v, v2, v_aux, v_aux2;
	
	if (tamanho_lista(componentes) == 1){
		return 0;
	}
	
	
	// fila de prioridades <- raiz (lista componentes) verificar se o primeiro elemento da lista é realmente a raiz
	
	for(no n = primeiro_no(componentes); n; n = proximo_no(n)){
		v = conteudo(n);
		for(no n2 = primeiro_no(v->vizinhos); n2; n2 = proximo_no(n2)){
			v2 = conteudo(n2);
		}
		
	}
	aux = primeiro_no(componentes);
	v_aux = conteudo(aux);
	v_aux->visitado = 1;
	v_aux->peso = 0;
	lista prioridades = constroi_lista();


	for (aux2 = primeiro_no(v_aux->vizinhos); aux2; aux2 = proximo_no(aux2)) {
		v_aux2 = conteudo(aux2);
		v_aux2->visitado = 1;
		v_aux2->peso = v_aux->peso +1;
		insere_lista(v_aux2, prioridades);
	}

	while (tamanho_lista(prioridades)){
		aux = primeiro_no(prioridades);
		v_aux = conteudo(aux);
		for (aux2 = primeiro_no(v_aux->vizinhos); aux2; aux2 = proximo_no(aux2)) {
			v_aux2 = conteudo(aux2);
			if (!v_aux2->visitado) {
				v_aux2->visitado = 1;
				v_aux2->peso = v_aux->peso +1;
				insere_lista(v_aux2, prioridades);
			}
		}
		remove_no_dry(prioridades, aux, NULL);
	}
	int maior = 0;
	for (aux = primeiro_no(componentes); aux; aux = proximo_no(aux)) {
		v_aux = conteudo(aux);
		if (v_aux->peso > maior) {
			maior = v_aux->peso;
		}
	}
	free(prioridades);
	return maior;
}

void otimiza_prioridades (lista l, struct no *pai){
	no aux, p;
	p = primeiro_no(l);
	 while (aux = proximo_no(p)) {
		if (pai != p){
			remove_no(l, p, destroi_nobusca);
		}
			p = aux;

	 }
}

void insere_lista_priori(nobusca nb, lista prioridades, no pai){
	no aux;
	if(tamanho_lista(prioridades) > MAX_PRIORI){
		aux = maiorfn(prioridades, pai);
		remove_no(prioridades, aux, destroi_nobusca);
			
	}
	insere_lista(nb, prioridades);
}

lista A_estrela (tmapa *m){
	
	no aux, aux2, nocomponente;
	vertice v_aux, v_aux2;
	nobusca nob, auxnob, pai;
	lista prioridades = constroi_lista();
	lista passos;
	int *passo;
	grafo g = le_grafo(m);
	grafo g2;
	
	lista componentes = cria_componentes(g);
	
	aux = primeiro_no(componentes);
	v_aux = conteudo(aux);
	for (aux2 = primeiro_no(v_aux->vizinhos); aux2; aux2 = proximo_no(aux2)) {
		v_aux2 = conteudo(aux2);
		g2 = copia_grafo(g);
		passo = malloc(sizeof(int));
		*passo = busca_indice_cor(g->vertices, v_aux2->indice);
		joga(*passo, v_aux->indice, g2);
		nob = constroi_nobusca();
		nob->hn = h(componentes);
		nob->g = 0;
		insere_lista(passo, nob->passos);
		nob->grafomapa = g2;
		insere_lista(nob, prioridades);
		
	}
	destroi_lista(componentes, destroi_vertice);
	int menor;
	while (tamanho_lista(prioridades)){
		aux = menorfn(prioridades);
		pai = conteudo(aux);
		componentes = cria_componentes(pai->grafomapa);
		nocomponente = primeiro_no(componentes);
		v_aux = conteudo(nocomponente);
		if (tamanho_lista(componentes) == 1){
			lista resultado = constroi_lista();
			copia_lista_cor(pai->passos, resultado);
			destroi_lista(prioridades, destroi_nobusca);
			destroi_lista(componentes, destroi_vertice);
			return resultado;
		}
		menor = (pai->g + pai->hn)*3;
		for (aux2 = primeiro_no(v_aux->vizinhos); aux2; aux2 = proximo_no(aux2)) {
			v_aux2 = conteudo(aux2);
			v_aux2->comunidade++;
			g2 = copia_grafo(pai->grafomapa);
			passo = malloc(sizeof(int));
			*passo = busca_indice_cor(g->vertices, v_aux2->indice);
			joga(*passo, v_aux->indice, g2);
			nob = constroi_nobusca();
			nob->hn = h(componentes);
			nob->g = pai->g +1;
			
			copia_lista_cor(pai->passos, nob->passos);
			insere_lista(passo, nob->passos);
			nob->grafomapa = g2;
			if((nob->hn + nob->g) < menor){
				menor = nob->hn + nob->g;
				insere_lista(nob, prioridades);
			}
			else{
				destroi_nobusca(nob);
			}
		}
		
		remove_no(prioridades, aux, destroi_nobusca);
		destroi_lista(componentes, destroi_vertice);
	}

}

int indices_iguais(lista componentes){
	no aux;
	vertice v_aux;
	for (aux = primeiro_no(componentes); aux; aux = proximo_no(aux)) {
		v_aux = conteudo(aux);
		if(v_aux->indice != 1)
			return 0;
	}
	return 1;
}

lista A_estrela_simples (tmapa *m){
	
	no aux;
	int* v_aux;
	grafo g = le_grafo(m);
	grafo g2;
	
	lista componentes = cria_componentes(g);
	g2 = copia_grafo(g);
	lista resultado = constroi_lista();
	int *passo;
	lista indices = h_simples(componentes, g);
	
	while (!indices_iguais(componentes)){
		
		for (aux = primeiro_no(indices); aux; aux = proximo_no(aux)) {
			v_aux = conteudo(aux);
			passo = malloc(sizeof(int));
			*passo = *v_aux;
			joga(*passo, 1, g);
			insere_lista(passo, resultado);
			destroi_lista(componentes, destroi_vertice);
			componentes = cria_componentes(g);
			
		}
		indices = h_simples(componentes, g);

	}
	destroi_grafo(g);
	return resultado;
	


}

void joga (int cor, int indice, grafo g){
	vertice novo;
	for (no n = primeiro_no(g->vertices); n; n=proximo_no(n)) {
	 	novo = conteudo(n);
		if (novo->indice == indice)
			novo->cor = cor;
	 }
}

void joga_lista (int cor, int indice, lista g, grafo graf){
	vertice novo, novo2;
	for (no n = primeiro_no(g); n; n=proximo_no(n)) { // percorre cada membro dos componentes
	 	novo = conteudo(n);
	 	for (no n2 = primeiro_no(novo->vizinhos); n2; n2=proximo_no(n2)) { // percorre os vizinhos desse membro
	 		novo2 = conteudo(n2);
	 		if ((novo2->indice == 1) && (busca_indice_cor(graf->vertices, novo->indice) == cor)){ // se o vizinho é raiz ou minha cor é igual a cor que está sendo pintada
	 			joga(cor, novo->indice, graf); // grafo pintar todos os vertices que eu represento da cor sendo pintada (colocar indice 1 da raiz)
	 			novo->indice = 0; // lista de componentes
	 		}
	 	}
	 	
	}
	for (no n = primeiro_no(g); n; n=proximo_no(n)) {
	 	novo = conteudo(n);
		if (novo->indice == 0)
	 		novo->indice =1;
	}
}

// maior f(N) que tem o maior H(n) 
no maiorfn (lista l, no pai){
	nobusca novo, maior;
	no aux = primeiro_no(l);
	maior = conteudo(aux);
	int maiorv = (maior->g + maior->hn);
	for (no n_lista = primeiro_no(l); n_lista; n_lista=proximo_no(n_lista)) {
	 	novo = conteudo(n_lista);
	 	
		if(pai == n_lista){
			continue;
		}
		if ( ((novo->g + novo->hn) > maiorv) && pai != n_lista){
			aux = n_lista;
			maiorv = (novo->g + novo->hn);
		}
		
	 }
	 return aux;
}

no menorfn (lista l){
	nobusca novo, menor;
	no aux = primeiro_no(l);
	menor = conteudo(aux);
	int menorv = (menor->g + menor->hn);
	for (no n_lista = primeiro_no(l); n_lista; n_lista=proximo_no(n_lista)) {
	 	novo = conteudo(n_lista);
		if ((novo->g + novo->hn) < menorv){
			aux = n_lista;
			menorv = (novo->g + novo->hn);
		}
	 }
	 return aux;

}





void copia_lista_cor(lista origem, lista destino) {
	if(!origem){
		return;
	}
	int *aux, *aux2;
	for (no n = primeiro_no(origem); n; n = proximo_no(n)) {
		aux = conteudo(n);
		aux2 = malloc(sizeof(int));
		*aux2 = *aux;
		insere_lista(aux2, destino);	
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
	// DADOS DA STRUCT GRAFO INTEIRO
	g_copy->nc = g->nc;
	g_copy->nl = g->nl;
	g_copy->cores = g->cores;

	no v_entrada;
	// Gera copias de TODOS os vértices
	for (v_entrada = primeiro_no(g->vertices); v_entrada; v_entrada = proximo_no(v_entrada)) {
		v = constroi_vertice();
		temp_v = conteudo(v_entrada);
		v->visitado = temp_v->visitado;
		v->l = temp_v->l;
		v->c = temp_v->c;
		v->cor = temp_v->cor;
		v->comunidade = temp_v->comunidade;
		v->peso = temp_v->peso;
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
			v2 = busca_vertice_mapa(g_copy->vertices, temp_v2->l, temp_v2->c);
			insere_lista(v2, v->vizinhos);
		}
	}
	return g_copy;
}
