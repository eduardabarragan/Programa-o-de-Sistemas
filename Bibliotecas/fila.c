#include "fila.h"
#include <stdlib.h>
#include <stdio.h>

void inicializa_fila(struct fila *f){
    f->inicio = NULL;
    f->fim = NULL;
}

int fila_vazia(struct fila *f){
    return f->inicio == NULL;
}

int insere_fila(struct fila *f, int valor){
    struct no_fila *novo;
    novo = (struct no_fila *)malloc(sizeof(struct no_fila));
    if(novo == NULL){
        return 0;
    }
    if(f->fim == NULL){
        f->inicio = novo;
        f->fim = novo;
    } else {
        f->fim->prox = novo;
        f->fim = novo;
    }
    novo ->valor = valor;
    novo->prox = NULL;
    return 1;
}

int elemento_topo(struct fila *f){
    if (fila_vazia(f)) {
        return -999;
    }
    else {
        return f->inicio->valor;
    }
}

int remove_fila(struct fila *f, int *retorno){
    if(fila_vazia(f)){
        return -999;
    }
    struct no_fila *del = f->inicio;
    *retorno = del->valor;
    f->inicio = del->prox;
    free(del);
    return *retorno;
}

void mostra_fila(struct fila *f){
    struct no_fila *aux = f->inicio;
    while(aux != NULL){
        printf("%d ", aux->valor);
        aux = aux->prox;
    }
    printf("\n");
}

struct no_fila *busca_fila(struct fila *f, int v){
    struct no_fila *aux = f->inicio;

    while(aux != NULL && aux->valor != v){
        aux = aux->prox;
    }     
    if (aux != NULL){
        return aux;
    }
}