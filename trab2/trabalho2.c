#include <stdio.h>
#include <string.h>   // para usar a função strcmp() que compara duas strings
#include <stdlib.h>   // para usar a função exit() que interrompe (aborta) a execução do programa.
#include "filaHierarquica.h" // Inclua o cabeçalho da fila hierárquica

typedef unsigned char Tpixel; // tipo Tpixel para valores em [0, 255]

typedef struct TPpgm { /// descritor para imagens tipo PGM
    char tipo[3]; /// "P1", "P2", "P3"
    int w;     // largura da imagem em pixels (TC - Total de pixels em cada coluna da imagem)
    int h;     // altura da imagem em pixel   (TL - Total de pixels em cada linha da imagem )
    int max;   // valor de brilho máximo registrado na imagem (use 255 sempre)
    int pixlen;    // total de pixels (em todas as bandas) da imagem
    Tpixel *pix; // ponteiro para o reticulado (matriz) da imagem alocada como um vetor.
} TPpgm;

void ReadPGM(const char *fname, TPpgm *img) {
    FILE *fd;

    printf("\n ..... (Abrindo ARQUIVO )-->%s.", fname);
    fd = fopen(fname, "r");   // modo "r" --> abrir somente para leitura
    if (!fd) {
        printf("\n ERRO: Incapaz de abrir arquivo: %s.\n\n", fname);
        exit(1);
    }
    printf("\n ..... (Abriu ARQUIVO )-->%s.", fname);
    fscanf (fd, "%s\n", img->tipo);
    // Lida com comentários no cabeçalho (descomente se precisar)
    /*
    char C[256];
    fscanf (fd, "%s", C);
    while (C[0] == '#') {
        fgets (C, 255, fd);
        fscanf (fd, "%s", C);
    }
    sscanf (C, "%d", &img->w);
    fscanf (fd, "%d\n", &img->h);
    */
    fscanf (fd, "%d\n", &img->w); /// largura da imagem
    fscanf (fd, "%d\n", &img->h); /// altura da imagem

    if (((strcmp(img->tipo, "P1") != 0))) { // se a imagem não é binária (P1), LER o
        fscanf (fd, "%d\n", &img->max); /// valor de brilho máximo na imagem
    }

    img->pixlen = 1*(img->w * img->h); // cada pixel tem 1 componente (valor de brilho)

    if (((strcmp(img->tipo, "P3")) == 0)) { /// se a imagem é colorida
        img->pixlen = 3*(img->w * img->h); // cada pixel tem 3 componentes (R,G,B)
    }
    printf("\n ..... (Total de pixels --> %d.", img->pixlen);
    printf("\n ..... (Leu Cabecalho )-->%s.", fname);
    int paux;
    img->pix = (Tpixel*)malloc(sizeof(Tpixel) * (img->pixlen));
    if (!img->pix) {
        printf("\n ERRO: Falha na alocação de memória para pixels da imagem PGM/PPM: %s.\n", fname);
        exit(1);
    }

    for (int k=0; k<img->pixlen; k++){
        fscanf (fd, "%d", &paux);
        img->pix[k] = (Tpixel) paux;
    }
    printf("\n ..... (Leu ARQUIVO )-->%s.", fname);
    fclose(fd);
}

void WritePGM(const char *fname, TPpgm *img) {
    FILE *fd;

    fd = fopen(fname, "w");   /// modo "w" --> abrir para escrita
    if (!fd) {
        printf("\n ERRO: Incapaz de abrir arquivo para escrita: %s.\n", fname);
        exit(1);
    }
    printf("\n ..... (ARQUIVO Aberto)-->%s.", fname);

    fprintf(fd, "%s\n", img->tipo); /// assinatura PGM: "P3", "P2" ou "P1"
    fprintf(fd,"%d %d\n", img->w, img->h);

    if (strcmp(img->tipo, "P1") != 0) { // a imagem não é binária?
            fprintf(fd,"%d\n", img->max); // valor de brilho máximo na imagem (usa o do struct)
    }

    /// gravar no arquivo os valores dos pixels da imagem computada
    for (unsigned int k = 0; k < img->pixlen; k++) {
        fprintf(fd,"%d ", img->pix[k]);
    }

    fclose(fd);
    printf("\n ..... (ARQUIVO GRAVADO)-->%s.", fname);
}

Tpixel GetPixel(TPpgm *img, int L, int C){
    if ((L >= img->h) ||  (C >= img->w) || ( L < 0) || (C < 0)) {
        printf("\n%d %d", L, C);
        printf("\n ERRO: Coordenada de imagem esta fora dos limites da grade da imagem.");
        exit(0);
    }
    Tpixel K;
    K = *((img->pix) + L*img->w + C);
    return (K);
}

int PutPixel(TPpgm *img, int L, int C, Tpixel v){
    if ((L >= img->h) ||  (C >= img->w) || ( L < 0) || (C < 0)) {
        printf("\n ERRO: Escrita na coordenada de imagem esta fora dos limites da grade da imagem.");
        exit(0);
    }
    *((img->pix) + L*img->w + C) = v;
    return (1);
}

// Função auxiliar interna para PutPixelRGB (agora usada no WatershedColor)
int PutPixelRGB_Internal(TPpgm *img, int L, int C, Tpixel r, Tpixel g, Tpixel b){
    if ((L >= img->h) ||  (C >= img->w) || ( L < 0) || (C < 0)) {
        printf("\n ERRO: Escrita na coordenada de imagem colorida esta fora dos limites.");
        exit(0);
    }
    *((img->pix) + 3*(L*img->w + C)) = r;
    *((img->pix) + 3*(L*img->w + C) + 1) = g;
    *((img->pix) + 3*(L*img->w + C) + 2) = b;
    return (1);
}

void RGB2Gray(TPpgm *f, TPpgm *g){
    if(strcmp(f->tipo, "P3") != 0){
        printf("\n ERRO: A Imagem de entrada deve ser colorida para conversão para cinza.\n");
        exit(1);
    }
    *g = *f;
    g->tipo[1] = '2'; // transforma o campo tipo de P3 para P2
    g->pixlen = f->pixlen/3; // Um terço dos pixels originais (R,G,B -> Cinza)

    g->pix = (Tpixel*) malloc (sizeof(Tpixel) * g->pixlen);
    if (!g->pix) {
        printf("\n ERRO: Falha na alocação de memória para pixels da imagem cinza.\n");
        exit(1);
    }

    int k=0;
    Tpixel paux;
    for(int j = 0; j < f->pixlen; j+=3){
        paux = (Tpixel)(((int) f->pix[j] + f->pix[j+1] + f->pix[j+2])/3);
        g->pix[k]=paux;
        k++;
    }
}

void imgInv(TPpgm *f, TPpgm *g) {
    *g = *f;
    g->pix = (Tpixel*)malloc(sizeof(Tpixel) * (g->pixlen));
    if (!g->pix) {
        printf("\n ERRO: Falha na alocação de memória para pixels da imagem invertida.\n");
        exit(1);
    }
    Tpixel paux = 1;
    if (strcmp(g->tipo, "P1") != 0) {
        paux = 255;
    }

    for (unsigned int k = 0; k < g->pixlen; k++) {
        g->pix[k] = paux - f->pix[k];
    }
}

void morphdil(TPpgm *f, TPpgm *g) {
    *g = *f;
    if (strcmp(g->tipo, "P3") == 0) {
        printf("\n ERRO: NÃO é possivel aplicar operador morfologico sobre dados de imagens coloridas \n");
        printf("\n ... Tipo da imagem PGM = %s \n", g->tipo);
        exit(1);
    }
    g->pix = (Tpixel*)calloc(sizeof(Tpixel), (g->pixlen));
    if (!g->pix) {
        printf("\n ERRO: Falha na alocação de memória para pixels de dilatação.\n");
        exit(1);
    }

    Tpixel mx, paux;
    for (int L = 1; L < g->h-1; L++) {
        for (int C = 1; C < g->w-1; C++) {
            mx = 0;
            for (int k = -1; k <= 1; k++) {
                for (int j = -1; j <= 1; j++) {
                    paux = GetPixel(f, L+k, C+j);
                    if(paux > mx) {
                        mx = paux;
                    }
                }
            }
            PutPixel(g, L, C, mx);
        }
    }
}

void morphero(TPpgm *f, TPpgm *g) {
    *g = *f;
    if (strcmp(g->tipo, "P3") == 0) {
        printf("\n ERRO: NÃO é possivel aplicar operador morfologico sobre dados de imagens coloridas \n");
        printf("\n ... Tipo da imagem PGM = %s \n", g->tipo);
        exit(1);
    }
    g->pix = (Tpixel*)calloc(sizeof(Tpixel), (g->pixlen));
    if (!g->pix) {
        printf("\n ERRO: Falha na alocação de memória para pixels de erosão.\n");
        exit(1);
    }

    Tpixel mn, paux;
    for (int L = 1; L < g->h-1; L++) {
        for (int C = 1; C < g->w-1; C++) {
            mn = 255;
            for (int k = -1; k <= 1; k++) {
                for (int j = -1; j <= 1; j++) {
                    paux = GetPixel(f, L+k, C+j);
                    if(paux < mn) {
                        mn = paux;
                    }
                }
            }
            PutPixel(g, L, C, mn);
        }
    }
}

void morphgrad(TPpgm *f, TPpgm *g) {
    *g = *f;
    if (strcmp(g->tipo, "P3") == 0) {
        printf("\n ERRO: NÃO é possivel aplicar operador morfologico sobre dados de imagens coloridas \n");
        printf("\n ... Tipo da imagem PGM = %s \n", g->tipo);
        exit(1);
    }
    g->pix = (Tpixel*)calloc(sizeof(Tpixel), (g->pixlen));
    if (!g->pix) {
        printf("\n ERRO: Falha na alocação de memória para pixels de gradiente.\n");
        exit(1);
    }

    Tpixel mn, mx, paux, grad_val;
    for (int L = 1; L < g->h-1; L++) {
        for (int C = 1; C < g->w-1; C++) {
            mn = 255;
            mx = 0;
            for (int k = -1; k <= 1; k++) {
                for (int j = -1; j <= 1; j++) {
                    paux = GetPixel(f, L+k, C+j);
                    if(paux < mn) {
                        mn = paux;
                    } else if (paux > mx) mx = paux;
                }
            }
            grad_val = mx - mn;
            PutPixel(g, L, C, grad_val);
        }
    }
}

int* ReadMarkers(const char *fname, int *tm){
    FILE *fd;
    fd = fopen(fname, "r");
    if (!fd) {
        printf("\n ERRO: Incapaz de abrir arquivo de marcadores: %s.\n", fname);
        exit(1);
    }
    printf("\n ..... (ARQUIVO Aberto)-->%s\n.", fname);

    fscanf(fd, "%d \n", tm);
    printf("Total de marcadores: %d \n", *tm);
    int *M = (int*)malloc(3*sizeof(int)*(*tm));
    if (!M) {
        printf("\n ERRO: Falha na alocação de memória para vetor de marcadores.\n");
        exit(1);
    }
    for (int k=0; k<(*tm)*3; k += 3){
      fscanf(fd, "%d %d %d",&M[k], &M[k+1], &M[k+2]);
    }
    fclose(fd);
    return M;
}

// --- Implementação da função Watershed com saída colorida ---
TPpgm WatershedColor(TPpgm *G, int *markers_data, int num_markers) {
    FilaHierarquica FH;
    inicializar_fila(&FH);

    TPpgm Ws;
    Ws.tipo[0] = 'P';
    Ws.tipo[1] = '3'; // A imagem de saída será colorida (PPM)
    Ws.tipo[2] = '\0';
    Ws.w = G->w;
    Ws.h = G->h;
    Ws.max = 255; // Valor máximo para componentes de cor
    Ws.pixlen = 3 * G->w * G->h; // 3 componentes por pixel (R, G, B)
    // calloc inicializa a memória com zeros, resultando em pixels pretos
    Ws.pix = (Tpixel*)calloc(sizeof(Tpixel), Ws.pixlen);
    if (!Ws.pix) {
        printf("\n ERRO: Falha na alocação de memória para pixels da imagem Watershed Color.\n");
        exit(1);
    }

    // Inicializa os marcadores na fila e na imagem de saída colorida
    for (int i = 0; i < num_markers; i++) {
        Pixel m;
        m.linha = markers_data[3 * i + 0];
        m.coluna = markers_data[3 * i + 1];
        m.rotulo = markers_data[3 * i + 2];

        // Garante que o marcador está dentro dos limites da imagem
        if (m.linha >= 0 && m.linha < G->h && m.coluna >= 0 && m.coluna < G->w) {
            // Enfileira o marcador com o valor do gradiente como prioridade
            enfileirar(&FH, m, GetPixel(G, m.linha, m.coluna));

            // Define a cor do marcador na imagem de saída Ws
            Tpixel rr = 0, gg = 0, bb = 0;
            switch (m.rotulo) {
                case 1: rr=255; gg=0; bb=0; break;   // Vermelho
                case 2: rr=0; gg=255; bb=0; break;   // Verde
                case 3: rr=0; gg=0; bb=255; break;   // Azul
                case 4: rr=255; gg=127; bb=0; break; // Laranja
                case 5: rr=255; gg=255; bb=255; break; // Branco
                case 6: rr=127; gg=127; bb=127; break; // Cinza Médio
                // Adicionado tratamento explícito para o rótulo 255 (geralmente bacias)
                case 255: rr=0; gg=0; bb=0; break; // Bacias (ou fronteiras) de preto
                default:
                    // Para rótulos não mapeados, use preto e avise (se não for 0)
                    if (m.rotulo != 0) { // Rótulo 0 é usado para não marcado em algumas abordagens, não avise para ele.
                        printf("Aviso: Rótulo %d do marcador não mapeado para cor.\n", m.rotulo);
                    }
                    rr=0; gg=0; bb=0;
                    break;
            }
            PutPixelRGB_Internal(&Ws, m.linha, m.coluna, rr, gg, bb);
        }
    }

    // Vetores de deslocamento para 8-conectividade
    int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    // Loop principal do algoritmo Watershed
    while (!fila_vazia(&FH)) {
        Pixel Pix = desenfileirar(&FH);

        // Determina a cor do pixel atual (baseada no rótulo que ele carrega)
        Tpixel pix_r = 0, pix_g = 0, pix_b = 0;
        switch (Pix.rotulo) {
            case 1: pix_r=255; pix_g=0; pix_b=0; break;
            case 2: pix_r=0; pix_g=255; pix_b=0; break;
            case 3: pix_r=0; pix_g=0; pix_b=255; break;
            case 4: pix_r=255; pix_g=127; pix_b=0; break;
            case 5: pix_r=255; pix_g=255; pix_b=255; break;
            case 6: pix_r=127; pix_g=127; pix_b=127; break;
            case 255: pix_r=0; pix_g=0; pix_b=0; break; // Bacias (ou fronteiras) de preto
            default:
                if (Pix.rotulo != 0) {
                     printf("Aviso: Rótulo %d atribuído a pixel não mapeado para cor durante a inundação.\n", Pix.rotulo);
                }
                pix_r=0; pix_g=0; pix_b=0;
                break;
        }

        // Explora os vizinhos
        for (int i = 0; i < 8; i++) {
            int nr = Pix.linha + dr[i];
            int nc = Pix.coluna + dc[i];

            // Verifica se o vizinho está dentro dos limites da imagem
            if (nr >= 0 && nr < G->h && nc >= 0 && nc < G->w) {
                // Verifica se o pixel vizinho ainda não foi rotulado (cor preta na imagem Ws)
                // A condição Ws.pix[...] == 0 verifica se o componente R (primeiro componente) é 0.
                // Como inicializamos com calloc, (0,0,0) significa não rotulado.
                if (Ws.pix[3 * (nr * Ws.w + nc)] == 0 &&
                    Ws.pix[3 * (nr * Ws.w + nc) + 1] == 0 &&
                    Ws.pix[3 * (nr * Ws.w + nc) + 2] == 0) {

                    Pixel vizinho;
                    vizinho.linha = nr;
                    vizinho.coluna = nc;
                    vizinho.rotulo = Pix.rotulo; // O vizinho recebe o rótulo do pixel atual

                    // Pinta o vizinho na imagem Ws
                    PutPixelRGB_Internal(&Ws, vizinho.linha, vizinho.coluna, pix_r, pix_g, pix_b);
                    // Enfileira o vizinho com sua própria prioridade de gradiente
                    enfileirar(&FH, vizinho, GetPixel(G, vizinho.linha, vizinho.coluna));
                }
            }
        }
    }
    return Ws;
}

int main(){
    const char* NimgO = "img01.ppm"; // imagem original
    const char* NimgGray = "img01Gray.pgm";
    const char *NimgGrad = "img01Grad.pgm";
    const char *fmkr = "ColorMarkers.txt";
    const char *NimgWsColor = "imgWatershedColor.ppm"; // Saída colorida do Watershed

    int tm;
    int *M;

    M = ReadMarkers(fmkr,&tm);

    // Para fins de depuração, imprime os marcadores lidos
    printf("\nMarcadores lidos:\n");
    for (int k=0; k<3*tm; k+=3){
        printf("Linha: %d, Coluna: %d, Rotulo: %d \n",M[k], M[k+1], M[k+2]);
    }

    TPpgm imgO, imgG, grad, imgWsColor;

    // 1. Ler a imagem original
    ReadPGM(NimgO, &imgO);

    // 2. Converter imagem original para tons de cinza
    RGB2Gray(&imgO, &imgG);
    WritePGM(NimgGray, &imgG); // Salvar imagem em tons de cinza

    // 3. Calcular a imagem gradiente (continua sendo em tons de cinza)
    // O algoritmo Watershed precisa de uma imagem de "superfície" para inundar.
    // O gradiente é ideal pois as "vales" (bordas) terão valores baixos.
    morphgrad(&imgG, &grad);
    WritePGM(NimgGrad, &grad); // Salvar imagem gradiente

    // 4. Executar o Watershed colorido usando a imagem gradiente e os marcadores
    imgWsColor = WatershedColor(&grad, M, tm);
    WritePGM(NimgWsColor, &imgWsColor); // Salvar a imagem Watershed colorida

    // 5. Liberar memória alocada para evitar vazamentos
    free(imgO.pix);
    free(imgG.pix);
    free(grad.pix);
    free(imgWsColor.pix);
    free(M);

    printf("\n\nProcessamento Watershed concluído. Verifique %s\n", NimgWsColor);

    return 0;
}