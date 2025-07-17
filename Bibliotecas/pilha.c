#include "pilha.h"
#include <stdlib.h>
#include <stdio.h>

void inicializa_pilha(struct pilha *p){
    p->topo = NULL;
}
int pilha_vazia(struct pilha *p){
    return p->topo == NULL;
}
int empilha(struct pilha *p, int valor){
    struct no_pilha *novo;
    novo = (struct no_pilha *)malloc(sizeof(struct no_pilha));
    if(novo == NULL){
        return 0;
    }
    novo->valor = valor;
    novo->prox = p->topo;
    p->topo = novo;
    return 1;
}
int desempilha(struct pilha *p, int *retorno){
    if(pilha_vazia(p)){
        return 0;
    }
    struct no_pilha *del = p->topo;
    *retorno = del->valor;
    p->topo = del->prox;
    free(del);
    return 1;
}

void mostra_pilha(struct pilha *p){
    struct no_pilha *aux = p->topo;
    while(aux != NULL){
        printf("%d ", aux->valor);
        aux = aux->prox;
    }
    printf("\n");
}
