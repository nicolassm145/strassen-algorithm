#include <stdio.h>
#include <time.h>
#include "matrizFunc.c"  // Inclui as funções auxiliares para manipulação de matrizes

int main() {
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    char format[3];
    int n;
    int colorMax;
    int rgb = 3;

    char filepath[100];

    // Garante que não ocorra estouro de buffer
    snprintf(filepath, sizeof(filepath), "../test cases/10.in");
    FILE *file = fopen(filepath, "r");

    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Lê o cabeçalho do arquivo PPM
    fscanf(file, "%s", format);      // Formato da imagem "P3"
    fscanf(file, "%d %d", &n, &n);   // Dimensões da imagem (n x n)
    fscanf(file, "%d", &colorMax);   // Valor máximo de cor


    // Aloca e lê a matriz de pixels (matrizA) do arquivo
    int ***matrizA = alocaMatriz(n, rgb);
    int ***matrizB = alocaMatriz(n, rgb);

    leMatriz(file, matrizA, n, rgb);
    leMatriz(file, matrizB, n, rgb);


    int ***matrizResult = algoritmoStrassen(matrizA, matrizB, n, rgb);

    // Output
    printf("%s\n", format);
    printf("%d %d\n", n, n);
    printf("%d", colorMax);

    imprimeMatriz(matrizResult, n, rgb);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\n\nTempo: %f segundos\n", cpu_time_used);

    liberaMatriz(matrizA, n);
    liberaMatriz(matrizB, n);
    liberaMatriz(matrizResult, n);

    fclose(file);
    return 0;
}