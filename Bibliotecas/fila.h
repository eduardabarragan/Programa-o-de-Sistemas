#ifndef __FILA_H__
#define __FILA_H__

struct no_fila{
    int valor;
    struct no_fila *prox;
};
struct fila{
    struct no_fila *inicio;
    struct no_fila *fim;
};

void inicializa_fila(struct fila *p);
int fila_vazia(struct fila *p);
int insere_fila(struct fila *p, int valor);
int remove_fila(struct fila *p, int *retorno);
void mostra_fila(struct fila *p);
int busca_fila(struct pilha *p, int v);
#endif