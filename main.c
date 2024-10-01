#include <stdio.h>
#include <time.h>
#include "matrizFunc.c"  // Inclui as funções auxiliares para manipulação de matrizes

int main() {
    // Variáveis para medir o tempo de execução
    clock_t start, end;
    double cpu_time_used;
    start = clock();  // Inicia a contagem do tempo

    // Declaração das matrizes
    int ***matrizA;    // Matriz que armazena os pixels RGB da imagem
    int ***matrizB;    // Matriz de filtro
    int ***matrizResult; // Matriz para armazenar o resultado da multiplicação


    // Variáveis relacionadas ao formato da imagem e tamanho da matriz
    char format[3];  // Armazena o formato do arquivo (ex: "P3" no caso de PPM)
    int n;           // Tamanho da matriz (matriz quadrada, então será n x n)
    int colorMax;    // Valor máximo da cor no arquivo PPM (ex: 255)
    int rgb = 3;     // Número de componentes RGB (sempre 3)

    char filepath[100]; // Caminho para o arquivo de entrada

    // Garante que não ocorra estouro de buffer ao definir o caminho do arquivo
    snprintf(filepath, sizeof(filepath), "../test cases/7.in");
    FILE *file = fopen(filepath, "r");  // Abre o arquivo para leitura

    // Verifica se o arquivo foi aberto corretamente
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Lê o cabeçalho do arquivo PPM
    fscanf(file, "%s", format);      // Formato da imagem (ex: "P3")
    fscanf(file, "%d %d", &n, &n);   // Dimensões da imagem (n x n)
    fscanf(file, "%d", &colorMax);   // Valor máximo de cor (ex: 255)

    // Exibe os dados lidos do cabeçalho
    printf("%s\n", format);
    printf("%d %d\n", n, n);
    printf("%d\n", colorMax);

    // Aloca e lê a matriz de pixels (matrizA) do arquivo
    matrizA = alocaMatriz(n, rgb);
    leMatriz(file, matrizA, n, rgb);

    // Aloca e lê a matriz de filtro (matrizB) do arquivo
    matrizB = alocaMatriz(n, rgb);
    leMatriz(file, matrizB, n, rgb);

    // Aloca a matriz para o resultado da multiplicação
    matrizResult = alocaMatriz(n, rgb);

    // Realiza a multiplicação clássica das matrizes
    multiplicaMatriz(matrizA, matrizB, matrizResult, n, rgb);
    //matrizResult = multiplicacaoMatrizesStrassen(matrizA, matrizB, n, rgb);

    // Imprime a matriz resultante após a multiplicação
    imprimeMatriz(matrizResult, n, rgb);

    // Medição do tempo de execução
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\n\nTempo: %f segundos\n", cpu_time_used);

    // Libera a memória alocada para as matrizes
    liberaMatriz(matrizA, n);
    liberaMatriz(matrizB, n);
    liberaMatriz(matrizResult, n);

    // Fecha o arquivo de entrada
    fclose(file);

    return 0;
}
