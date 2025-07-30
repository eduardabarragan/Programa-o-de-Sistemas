#ifndef FILA_HIERARQUICA_H
#define FILA_HIERARQUICA_H

// Estrutura para representar um pixel/marcador
typedef struct {
    int linha;
    int coluna;
    int rotulo; // Rótulo do marcador, ou 0 se não marcado
} Pixel;

// Nó da fila
typedef struct FilaNo {
    Pixel dado;
    struct FilaNo* prox;
} FilaNo;

// Estrutura da fila hierárquica
typedef struct {
    FilaNo* inicio[256]; // Vetor de ponteiros para o início de cada nível
    FilaNo* fim[256];    // Vetor de ponteiros para o fim de cada nível
    int menor_nivel_ativo; // O menor nível de hierarquia que contém elementos
} FilaHierarquica;

// Funções da fila hierárquica
void inicializar_fila(FilaHierarquica *fh);
int fila_vazia(FilaHierarquica *fh);
void enfileirar(FilaHierarquica *fh, Pixel p, int hierarquia);
Pixel desenfileirar(FilaHierarquica *fh);
void escreveFila(FilaHierarquica *fh); // Não é estritamente usada no watershed, mas mantida.
Pixel primeiroDaFila(FilaHierarquica *fh); // Não é estritamente usada no watershed, mas mantida.
int buscaNaFila(FilaHierarquica *fh, Pixel p_procurado); // Não é estritamente usada no watershed, mas mantida.

#endif // FILA_HIERARQUICA_H