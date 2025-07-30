#ifndef __PILHA_H__
#define __PILHA_H__
struct no_pilha{
    int valor;
    struct no_pilha *prox;
};
struct pilha{
    struct no_pilha *topo;
};
void inicializa_pilha(struct pilha *p);
int pilha_vazia(struct pilha *p);
int empilha(struct pilha *p, int valor);
int desempilha(struct pilha *p, int *retorno);
//int topo(struct pilha *p);
void mostra_pilha(struct pilha *p);
//int busca_pilha(struct pilha *p, int v);
#endif