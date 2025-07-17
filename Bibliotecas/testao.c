#include "fila.h"
#include <stdio.h>

void main() {
    struct fila f;
    int r;
    inicializa_fila(&f);
    insere_fila(&f, 10);
    int numeros [] = {20,30,40,50};

    for (int i =0; i < sizeof(numeros); i++){
        insere_fila(&f, numeros[i]);
    }
    printf("Topo: %d\n", remove_fila(&f, &r));
    printf("Topo: %d\n", remove_fila(&f, &r));
}