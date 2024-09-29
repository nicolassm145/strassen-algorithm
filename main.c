#include <stdio.h>
#include <stdlib.h>

#include "matrizFunc.c"


int main() {
    // matrizA = Matriz dos pixel RGB
    // matrizB = Matriz de filtro
    int ***matrizA;
    int ***matrizB;
    int ***matResult;

    // n = Tamanho da matriz, em todos os casos são  de matriz quadradas então será n^2
    char format[3]; // P3
    int n;
    int colorMax;
    int rgb = 3;

    char filepath[100];

    // Garante que não ocorra estouro de buffer
    snprintf(filepath, sizeof(filepath), "../test cases/1.in");
    FILE *file = fopen(filepath, "r");

    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    fscanf(file, "%s", format);
    fscanf(file, "%d %d", &n, &n);
    fscanf(file, "%d", &colorMax);

    printf("%s\n", format);
    printf("%d %d\n", n, n);
    printf("%d\n", colorMax);

    // Manipulação das matrizes

    matrizA = alocaMatriz(n, rgb);
    leMatriz(file, matrizA, n, rgb);
    imprimeMatriz(matrizA, n, rgb);

    matrizB   = alocaMatriz(n, rgb);
    leMatriz(file, matrizB, n, rgb);
    imprimeMatriz(matrizA, n, rgb);






    fclose(file);
}