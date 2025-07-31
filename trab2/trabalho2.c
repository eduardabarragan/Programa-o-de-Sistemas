#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "filaHierarquica.h"

typedef unsigned char Tpixel;

typedef struct TPpgm {
    char tipo[3];
    int w;
    int h;
    int max;
    int pixlen;
    Tpixel *pix;
} TPpgm;

void ReadPGM(const char *fname, TPpgm *img) {
    FILE *fd = fopen(fname, "r");
    if (!fd) {
        printf("\nERRO: Não foi possível abrir o arquivo: %s\n", fname);
        exit(1);
    }

    fscanf(fd, "%s\n", img->tipo);
    fscanf(fd, "%d\n", &img->w);
    fscanf(fd, "%d\n", &img->h);

    if (strcmp(img->tipo, "P1") != 0) {
        fscanf(fd, "%d\n", &img->max);
    } else {
        img->max = 1;
    }

    img->pixlen = (strcmp(img->tipo, "P3") == 0) ? 3 * img->w * img->h : img->w * img->h;
    img->pix = (Tpixel*)malloc(img->pixlen * sizeof(Tpixel));
    if (!img->pix) {
        printf("\nERRO: Falha na alocação de memória\n");
        exit(1);
    }

    int paux;
    if (strcmp(img->tipo, "P1") == 0) {
        // Tratamento especial para imagens binárias P1
        char c;
        int k = 0;
        while (fscanf(fd, "%c", &c) != EOF) {
            if (c == '0' || c == '1') {
                img->pix[k++] = (c == '1') ? 1 : 0;
            }
            if (k >= img->pixlen) break;
        }
    } else {
        for (int k = 0; k < img->pixlen; k++) {
            fscanf(fd, "%d", &paux);
            img->pix[k] = (Tpixel)paux;
        }
    }
    fclose(fd);
}

void WritePGM(const char *fname, TPpgm *img) {
    FILE *fd = fopen(fname, "w");
    if (!fd) {
        printf("\n ERRO: Incapaz de abrir arquivo para escrita: %s.\n", fname);
        exit(1);
    }

    fprintf(fd, "%s\n", img->tipo);
    fprintf(fd, "%d %d\n", img->w, img->h);

    if (strcmp(img->tipo, "P1") != 0) {
        fprintf(fd, "%d\n", img->max);
    }

    if (strcmp(img->tipo, "P1") == 0) {
        // Formatação especial para imagens binárias
        for (int i = 0; i < img->h; i++) {
            for (int j = 0; j < img->w; j++) {
                fprintf(fd, "%d ", img->pix[i * img->w + j]);
            }
            fprintf(fd, "\n");
        }
    } else if (strcmp(img->tipo, "P3") == 0) {
        // Formatação para imagens coloridas
        for (int i = 0; i < img->pixlen; i += 3) {
            fprintf(fd, "%d %d %d ", img->pix[i], img->pix[i+1], img->pix[i+2]);
            if ((i/3) % img->w == img->w-1) fprintf(fd, "\n");
        }
    } else {
        // Formatação para tons de cinza
        for (int i = 0; i < img->pixlen; i++) {
            fprintf(fd, "%d ", img->pix[i]);
            if (i % img->w == img->w-1) fprintf(fd, "\n");
        }
    }

    fclose(fd);
    printf("\nArquivo gravado: %s", fname);
}

Tpixel GetPixel(TPpgm *img, int L, int C){
    if ((L >= img->h) ||  (C >= img->w) || ( L < 0) || (C < 0)) {
        return 0;
    }
    if (strcmp(img->tipo, "P3") == 0) {
        // Para imagens coloridas, retornamos o valor do primeiro canal (R)
        return img->pix[3*(L * img->w + C)];
    }
    return img->pix[L * img->w + C];
}

int PutPixel(TPpgm *img, int L, int C, Tpixel v){
    if ((L >= img->h) ||  (C >= img->w) || ( L < 0) || (C < 0)) {
        return 0;
    }
    if (strcmp(img->tipo, "P3") == 0) {
        // Para imagens coloridas, definimos todos os canais com o mesmo valor (tons de cinza)
        int index = 3*(L * img->w + C);
        img->pix[index] = v;
        img->pix[index+1] = v;
        img->pix[index+2] = v;
    } else {
        img->pix[L * img->w + C] = v;
    }
    return 1;
}

int PutPixelRGB_Internal(TPpgm *img, int L, int C, Tpixel r, Tpixel g, Tpixel b){
    if ((L >= img->h) ||  (C >= img->w) || ( L < 0) || (C < 0)) {
        return 0;
    }
    int index = 3*(L * img->w + C);
    img->pix[index] = r;
    img->pix[index+1] = g;
    img->pix[index+2] = b;
    return 1;
}

void RGB2Gray(TPpgm *f, TPpgm *g){
    if(strcmp(f->tipo, "P3") != 0){
        printf("\n ERRO: A Imagem de entrada deve ser colorida para conversão para cinza.\n");
        exit(1);
    }
    
    strcpy(g->tipo, "P2");
    g->w = f->w;
    g->h = f->h;
    g->max = f->max;
    g->pixlen = f->w * f->h;

    g->pix = (Tpixel*)malloc(g->pixlen * sizeof(Tpixel));
    if (!g->pix) {
        printf("\n ERRO: Falha na alocação de memória\n");
        exit(1);
    }

    for (int i = 0, k = 0; i < f->pixlen; i += 3, k++) {
        float gray = 0.299f * f->pix[i] + 
                     0.587f * f->pix[i+1] + 
                     0.114f * f->pix[i+2];
        g->pix[k] = (Tpixel)gray;
    }
}

void imgInv(TPpgm *f, TPpgm *g) {
    strcpy(g->tipo, f->tipo);
    g->w = f->w;
    g->h = f->h;
    g->max = f->max;
    g->pixlen = f->pixlen;

    g->pix = (Tpixel*)malloc(g->pixlen * sizeof(Tpixel));
    if (!g->pix) {
        printf("\n ERRO: Falha na alocação de memória\n");
        exit(1);
    }
    
    Tpixel paux = (strcmp(g->tipo, "P1") == 0) ? 1 : 255;
    for (int k = 0; k < g->pixlen; k++) {
        g->pix[k] = paux - f->pix[k];
    }
}

void morphdil(TPpgm *f, TPpgm *g) {
    strcpy(g->tipo, f->tipo);
    g->w = f->w;
    g->h = f->h;
    g->max = f->max;
    g->pixlen = f->pixlen;

    if (strcmp(g->tipo, "P3") == 0) {
        printf("\n ERRO: Operador morfológico não suporta imagens coloridas\n");
        exit(1);
    }
    
    g->pix = (Tpixel*)malloc(g->pixlen * sizeof(Tpixel));
    if (!g->pix) {
        printf("\n ERRO: Falha na alocação de memória\n");
        exit(1);
    }

    for (int L = 0; L < g->h; L++) {
        for (int C = 0; C < g->w; C++) {
            Tpixel mx = 0;
            for (int k = -1; k <= 1; k++) {
                for (int j = -1; j <= 1; j++) {
                    int nl = L + k;
                    int nc = C + j;
                    if (nl >= 0 && nl < f->h && nc >= 0 && nc < f->w) {
                        Tpixel paux = GetPixel(f, nl, nc);
                        if(paux > mx) mx = paux;
                    }
                }
            }
            PutPixel(g, L, C, mx);
        }
    }
}

void morphero(TPpgm *f, TPpgm *g) {
    strcpy(g->tipo, f->tipo);
    g->w = f->w;
    g->h = f->h;
    g->max = f->max;
    g->pixlen = f->pixlen;

    if (strcmp(g->tipo, "P3") == 0) {
        printf("\n ERRO: Operador morfológico não suporta imagens coloridas\n");
        exit(1);
    }
    
    g->pix = (Tpixel*)malloc(g->pixlen * sizeof(Tpixel));
    if (!g->pix) {
        printf("\n ERRO: Falha na alocação de memória\n");
        exit(1);
    }

    for (int L = 0; L < g->h; L++) {
        for (int C = 0; C < g->w; C++) {
            Tpixel mn = (strcmp(g->tipo, "P1") == 0) ? 1 : 255;
            for (int k = -1; k <= 1; k++) {
                for (int j = -1; j <= 1; j++) {
                    int nl = L + k;
                    int nc = C + j;
                    if (nl >= 0 && nl < f->h && nc >= 0 && nc < f->w) {
                        Tpixel paux = GetPixel(f, nl, nc);
                        if(paux < mn) mn = paux;
                    }
                }
            }
            PutPixel(g, L, C, mn);
        }
    }
}

void morphgrad(TPpgm *f, TPpgm *g) {
    strcpy(g->tipo, "P2");
    g->w = f->w;
    g->h = f->h;
    g->max = 255;
    g->pixlen = f->w * f->h;

    if (strcmp(f->tipo, "P3") == 0) {
        printf("\n ERRO: Gradiente morfológico requer imagem em tons de cinza\n");
        exit(1);
    }
    
    g->pix = (Tpixel*)malloc(g->pixlen * sizeof(Tpixel));
    if (!g->pix) {
        printf("\n ERRO: Falha na alocação de memória\n");
        exit(1);
    }

    for (int L = 0; L < g->h; L++) {
        for (int C = 0; C < g->w; C++) {
            Tpixel mn = 255, mx = 0;
            for (int k = -1; k <= 1; k++) {
                for (int j = -1; j <= 1; j++) {
                    int nl = L + k;
                    int nc = C + j;
                    if (nl >= 0 && nl < f->h && nc >= 0 && nc < f->w) {
                        Tpixel paux = GetPixel(f, nl, nc);
                        if(paux < mn) mn = paux;
                        if(paux > mx) mx = paux;
                    }
                }
            }
            PutPixel(g, L, C, mx - mn);
        }
    }
}

int* ReadMarkers(const char *fname, int *tm){
    FILE *fd = fopen(fname, "r");
    if (!fd) {
        printf("\n ERRO: Incapaz de abrir arquivo de marcadores: %s.\n", fname);
        exit(1);
    }

    fscanf(fd, "%d \n", tm);
    int *M = (int*)malloc(3 * sizeof(int) * (*tm));
    if (!M) {
        printf("\n ERRO: Falha na alocação de memória\n");
        exit(1);
    }
    
    for (int k = 0; k < (*tm)*3; k += 3){
        fscanf(fd, "%d %d %d", &M[k], &M[k+1], &M[k+2]);
    }
    fclose(fd);
    return M;
}


TPpgm WatershedColor(TPpgm *G, TPpgm *base, int *markers_data, int num_markers) {
    FilaHierarquica FH;
    inicializar_fila(&FH);

    TPpgm Ws;
    strcpy(Ws.tipo, "P3");
    Ws.w = base->w;
    Ws.h = base->h;
    Ws.max = 255;
    Ws.pixlen = 3 * base->w * base->h;
    Ws.pix = (Tpixel*)malloc(Ws.pixlen * sizeof(Tpixel));
    if (!Ws.pix) {
        printf("\n ERRO: Falha na alocação de memória\n");
        exit(1);
    }

    // 1. Copia a imagem base (tons de cinza) para a saída, convertendo para RGB
    for (int i = 0, j = 0; i < base->pixlen; i++, j += 3) {
        Tpixel gray_val = base->pix[i];
        Ws.pix[j] = gray_val;     // R
        Ws.pix[j+1] = gray_val;   // G
        Ws.pix[j+2] = gray_val;   // B
    }

    // 2. Estruturas auxiliares
    int *rotulo = (int*)calloc(base->w * base->h, sizeof(int));
    int *processado = (int*)calloc(base->w * base->h, sizeof(int));
    if (!rotulo || !processado) {
        printf("\n ERRO: Falha na alocação de memória\n");
        exit(1);
    }

    // 3. Processar marcadores iniciais
    for (int i = 0; i < num_markers; i++) {
        int L = markers_data[3*i];
        int C = markers_data[3*i+1];
        int R = markers_data[3*i+2];
        
        if (L >= 0 && L < base->h && C >= 0 && C < base->w) {
            int idx = L * base->w + C;
            rotulo[idx] = R;
            processado[idx] = 1;
            enfileirar(&FH, (Pixel){L, C, R}, GetPixel(G, L, C));
            
            // Aplica cor apenas no marcador inicial
            Tpixel r = 0, g = 0, b = 0;
            switch(R) {
                case 1: r = 255; break;    // Vermelho
                case 2: g = 255; break;    // Verde
                case 3: b = 255; break;    // Azul
                case 4: r = 255; g = 165; break; // Laranja
                case 5: r = g = b = 255; break;  // Branco
                case 6: r = g = b = 127; break;  // Cinza
                default: break;
            }
            PutPixelRGB_Internal(&Ws, L, C, r, g, b);
        }
    }

    // 4. Vetores de deslocamento (8-conectividade)
    int dr[] = {-1,-1,-1, 0,0, 1,1,1};
    int dc[] = {-1, 0, 1,-1,1,-1,0,1};

    // 5. Algoritmo de inundação modificado
    while (!fila_vazia(&FH)) {
        Pixel P = desenfileirar(&FH);
        int L = P.linha;
        int C = P.coluna;
        int R = P.rotulo;

        // 6. Processar vizinhos
        for (int i = 0; i < 8; i++) {
            int nL = L + dr[i];
            int nC = C + dc[i];
            
            if (nL < 0 || nL >= base->h || nC < 0 || nC >= base->w)
                continue;
            
            int idx = nL * base->w + nC;
            
            if (!processado[idx]) {
                processado[idx] = 1;
                rotulo[idx] = R;
                enfileirar(&FH, (Pixel){nL, nC, R}, GetPixel(G, nL, nC));
                
                // Aplica cor mantendo o brilho original
                Tpixel gray_val = GetPixel(base, nL, nC);
                Tpixel r = gray_val, g = gray_val, b = gray_val;
                
                switch(R) {
                    case 1: r = 255; g = b = gray_val/3; break;
                    case 2: g = 255; r = b = gray_val/3; break;
                    case 3: b = 255; r = g = gray_val/3; break;
                    case 4: r = 255; g = 165; b = gray_val/3; break;
                    case 5: r = g = b = 255; break;
                    case 6: r = g = b = 127; break;
                    default: break;
                }
                PutPixelRGB_Internal(&Ws, nL, nC, r, g, b);
            }
        }
    }

    free(rotulo);
    free(processado);
    return Ws;
}

void GaussianBlur(TPpgm *f, TPpgm *g) {
    // Kernel gaussiano 3x3
    float kernel[3][3] = {
        {1/16.0f, 2/16.0f, 1/16.0f},
        {2/16.0f, 4/16.0f, 2/16.0f},
        {1/16.0f, 2/16.0f, 1/16.0f}
    };

    strcpy(g->tipo, f->tipo);
    g->w = f->w;
    g->h = f->h;
    g->max = f->max;
    g->pixlen = f->pixlen;

    g->pix = (Tpixel*)malloc(g->pixlen * sizeof(Tpixel));
    if (!g->pix) {
        printf("\n ERRO: Falha na alocação de memória\n");
        exit(1);
    }

    for (int L = 1; L < g->h-1; L++) {
        for (int C = 1; C < g->w-1; C++) {
            float sum = 0.0;
            for (int k = -1; k <= 1; k++) {
                for (int j = -1; j <= 1; j++) {
                    Tpixel p = GetPixel(f, L+k, C+j);
                    sum += p * kernel[k+1][j+1];
                }
            }
            PutPixel(g, L, C, (Tpixel)sum);
        }
    }
}



int main() {
    const char* NimgO = "img01.ppm";
    const char* NimgGray = "img01Gray.pgm";
    const char* NimgSmooth = "img01Smooth.pgm";
    const char* NimgGrad = "img01Grad.pgm";
    const char* fmkr = "ColorMarkers.txt";
    const char* NimgWsColor = "imgWatershedColor.ppm";

    int tm;
    int *M = ReadMarkers(fmkr, &tm);

    printf("\nMarcadores lidos: %d\n", tm);
    for (int k = 0; k < tm*3; k += 3) {
        printf("Linha: %d, Coluna: %d, Rotulo: %d\n", M[k], M[k+1], M[k+2]);
    }

    TPpgm imgO, imgG, imgS, grad, imgWsColor;

    // 1. Ler imagem original
    ReadPGM(NimgO, &imgO);

    // 2. Converter para tons de cinza
    RGB2Gray(&imgO, &imgG);
    WritePGM(NimgGray, &imgG);

    // 3. Aplicar suavização Gaussiana
    GaussianBlur(&imgG, &imgS);
    WritePGM(NimgSmooth, &imgS);

    // 4. Calcular gradiente morfológico
    morphgrad(&imgS, &grad);
    WritePGM(NimgGrad, &grad);


    // 5. Executar watershed colorido SOBRE A IMAGEM GRAYSCALE
    imgWsColor = WatershedColor(&grad, &imgG, M, tm);
    WritePGM(NimgWsColor, &imgWsColor);

    // Liberar memória
    free(imgO.pix);
    free(imgG.pix);
    free(imgS.pix);
    free(grad.pix);
    free(imgWsColor.pix);
    free(M);

    printf("\nProcessamento concluído com sucesso!\n");
    return 0;
}