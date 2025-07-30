#include <stdlib.h>
#include <stdio.h>
#include "filaHierarquica.h"

// Inicializa a fila, zerando todos os ponteiros
void inicializar_fila(FilaHierarquica *fh) {
    for (int i = 0; i < 256; i++) {
        fh->inicio[i] = NULL;
        fh->fim[i] = NULL;
    }
    fh->menor_nivel_ativo = 256; // Um valor inválido inicial, indicando fila vazia
}

// Verifica se a fila está completamente vazia
int fila_vazia(FilaHierarquica *fh) {
    return fh->menor_nivel_ativo >= 256;
}

// Insere um elemento na fila de um nível de hierarquia específico
void enfileirar(FilaHierarquica *fh, Pixel p, int hierarquia) {
    // Garante que a hierarquia esteja dentro dos limites válidos [0, 255]
    if (hierarquia < 0) hierarquia = 0;
    if (hierarquia > 255) hierarquia = 255;

    FilaNo* novo_no = (FilaNo*)malloc(sizeof(FilaNo));
    if (!novo_no) {
        printf("Erro: Falha na alocação de memória para um nó da fila!\n");
        exit(1); // Aborta o programa se a alocação falhar
    }
    novo_no->dado = p;
    novo_no->prox = NULL;

    if (fh->inicio[hierarquia] == NULL) { // Fila para esta hierarquia estava vazia
        fh->inicio[hierarquia] = novo_no;
        fh->fim[hierarquia] = novo_no;
    } else { // Já havia elementos
        fh->fim[hierarquia]->prox = novo_no;
        fh->fim[hierarquia] = novo_no;
    }

    // Atualiza o menor nível ativo se necessário
    if (hierarquia < fh->menor_nivel_ativo) {
        fh->menor_nivel_ativo = hierarquia;
    }
}

// Remove um elemento do menor nível de hierarquia disponível
Pixel desenfileirar(FilaHierarquica *fh) {
    if (fila_vazia(fh)) {
        printf("Erro: Tentativa de desenfileirar de uma fila vazia!\n");
        // Retorna um pixel inválido ou trata o erro
        Pixel p_erro = {-1, -1, -1};
        return p_erro;
    }

    int nivel = fh->menor_nivel_ativo;
    FilaNo* no_removido = fh->inicio[nivel];
    Pixel dado_removido = no_removido->dado;

    fh->inicio[nivel] = no_removido->prox;
    if (fh->inicio[nivel] == NULL) { // A fila deste nível ficou vazia
        fh->fim[nivel] = NULL;

        // Procura o próximo menor nível não vazio
        int proximo_nivel = nivel + 1;
        while (proximo_nivel < 256 && fh->inicio[proximo_nivel] == NULL) {
            proximo_nivel++;
        }
        fh->menor_nivel_ativo = proximo_nivel;
    }

    free(no_removido);
    return dado_removido;
}

// Método para escrever/imprimir todos os elementos da fila (apenas para depuração)
void escreveFila(FilaHierarquica *fh) {
    if (fila_vazia(fh)) {
        printf("Fila hierarquica esta vazia.\n");
        return;
    }

    printf("=== Conteudo da Fila Hierarquica ===\n");

    for (int nivel = 0; nivel < 256; nivel++) {
        if (fh->inicio[nivel] != NULL) {
            printf("Nível %d: ", nivel);
            FilaNo* atual = fh->inicio[nivel];

            while (atual != NULL) {
                printf("(%d,%d,%d) ", atual->dado.linha, atual->dado.coluna, atual->dado.rotulo);
                atual = atual->prox;
            }
            printf("\n");
        }
    }
    printf("=====================================\n");
}

// Método para retornar o primeiro elemento não nulo da fila (sem remover)
Pixel primeiroDaFila(FilaHierarquica *fh) {
    if (fila_vazia(fh)) {
        printf("Erro: Fila hierárquica está vazia!\n");
        Pixel p_erro = {-1, -1, -1};
        return p_erro;
    }
    return fh->inicio[fh->menor_nivel_ativo]->dado;
}

// Método para buscar um elemento específico na fila
int buscaNaFila(FilaHierarquica *fh, Pixel p_procurado) {
    if (fila_vazia(fh)) {
        return 0; // Elemento não encontrado (fila vazia)
    }

    for (int nivel = 0; nivel < 256; nivel++) {
        if (fh->inicio[nivel] != NULL) {
            FilaNo* atual = fh->inicio[nivel];
            while (atual != NULL) {
                if (atual->dado.linha == p_procurado.linha &&
                    atual->dado.coluna == p_procurado.coluna &&
                    atual->dado.rotulo == p_procurado.rotulo) {
                    return 1; // Elemento encontrado
                    }
                atual = atual->prox;
            }
        }
    }
    return 0; // Elemento não encontrado
}