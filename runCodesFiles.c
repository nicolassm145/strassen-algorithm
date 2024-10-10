#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct rgb{
    int cor[3];
}rgb;

rgb **alocaMatriz(int tam);
void liberaMatriz(rgb **matriz, int tam);
void leMatriz(FILE *file, **matriz, int tam, int valorCor);
void imprimeMatriz(rgb **matriz, int tam, int valorCor);
void multiplicaMatriz(rgb **matrizA, rgb **matrizB, rgb **matrizResult, int tam, int valorCor);

rgb **somaMatriz (rgb **primeiraParte, rgb **segundaParte, int tam, int valorCor);
rgb **subtraiMatriz (rgb **primeiraParte, rgb **segundaParte, int tam, int valorCor);
void divideMatrizStrassen(rgb **matriz, rgb **quadranteA, rgb **quadranteB, rgb **quadranteC, rgb **quadranteD, int tam, int valorCor);
rgb **algoritmoStrassen (int ***matrizPixel, int ***matrizFiltro, int tam, int valorCor);

int main() {
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    char formato[3];
    int n;
    int colorMax;
    int valorCor = 3;

    char filepath[100];

    // Garante que não ocorra estouro de buffer
    snprintf(filepath, sizeof(filepath), "../test cases/10.in");
    FILE *file = fopen(filepath, "r");

    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Lê o cabeçalho do arquivo PPM
    fscanf(file, "%s", formato);      // Formato da imagem "P3"
    fscanf(file, "%d %d", &n, &n);   // Dimensões da imagem (n x n)
    fscanf(file, "%d", &colorMax);   // Valor máximo de cor

    // Aloca e lê a matriz de pixels (matrizA) do arquivo
    rgb **matrizA = alocaMatriz(n);
    rgb **matrizB = alocaMatriz(n);

    //leMatriz(file, matrizA, n, valorCor);
    //leMatriz(file, matrizB, n, valorCor);
    leMatriz(file, matrizA, n, valorCor);
    leMatriz(file, matrizB, n, valorCor);

    rgb **matrizResult = algoritmoStrassen(matrizA, matrizB, n, valorCor);

    // Output
    printf("%s\n", formato);
    printf("%d %d\n", n, n);
    printf("%d", colorMax);

    imprimeMatriz(matrizResult, n, valorCor);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\n\nTempo: %f segundos\n", cpu_time_used);

    liberaMatriz(matrizA, n);
    liberaMatriz(matrizB, n);
    liberaMatriz(matrizResult, n);

    fclose(file);
    return 0;
}



rgb **alocaMatriz(int tam) {
    rgb **matriz = malloc(sizeof(rgb *) * tam);
    for (int i = 0; i < tam; i++)
        matriz[i] = malloc(sizeof(rgb) * tam);
    return matriz;
}

void liberaMatriz(rgb **matriz, int tam) {
    for (int i = 0; i < tam; i++)
        free(matriz[i]);
    free(matriz);
}

void leMatriz(FILE *file,rgb **matriz, int tam, int valorCor) {
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            for (int k = 0; k < valorCor; k++) {
                fscanf(file, "%d", &matriz[i][j].cor[k]);
                //scanf("%d", &matriz[i][j].cor[k]);
            }
        }
    }
}


void imprimeMatriz(rgb **matriz, int tam, int valorCor) {
    for (int i = 0; i < tam; i++) {
        printf("\n");
        for (int j = 0; j < tam; j++) {
            for (int k = 0; k < valorCor; k++) {
                printf("%d ", matriz[i][j].cor[k]);
            }
        }
    }
}

// Multiplicação clássica de matriz
void multiplicaMatriz(rgb **matrizA, rgb **matrizB, rgb **matrizResult, int tam, int valorCor) {
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            for (int k = 0; k < valorCor; k++) {
                matrizResult[i][j].cor[k] = 0;
            }
            for (int l = 0; l < tam; l++) {
                for (int k = 0; k < valorCor; k++) {
                    matrizResult[i][j].cor[k] += matrizA[i][l].cor[k] * matrizB[l][j].cor[k];
                }
            }
        }
    }
}

// Funções para o strassen

rgb **somaMatriz (rgb **primeiraParte, rgb **segundaParte, int tam, int valorCor) {
    rgb **result = alocaMatriz(tam);
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            for (int k = 0; k < valorCor; k++) {
                result[i][j].cor[k] = primeiraParte[i][j].cor[k] + segundaParte[i][j].cor[k];
            }
        }
    }
    return result;
}

rgb **subtraiMatriz (rgb **primeiraParte, rgb **segundaParte, int tam, int valorCor) {
    rgb **result = alocaMatriz(tam);
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            for (int k = 0; k < valorCor; k++) {
                result[i][j].cor[k] = primeiraParte[i][j].cor[k] - segundaParte[i][j].cor[k];
            }
        }
    }
    return result;
}

// Divide a matriz em quatro quadrantes (submatrizes)
void divideMatrizStrassen(rgb **matriz, rgb **quadranteA, rgb **quadranteB, rgb **quadranteC, rgb **quadranteD, int tam, int valorCor) {
    //  Formato depois da divisão:
    //  | A   B |
    //  | C   D |
    //  Cada quadrante copia os elementos de sua determinada região
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            for (int k = 0; k < valorCor; k++) {
                quadranteA[i][j].cor[k] = matriz[i][j].cor[k];
                quadranteB[i][j].cor[k] = matriz[i][j + tam].cor[k];
                quadranteC[i][j].cor[k] = matriz[i + tam][j].cor[k];
                quadranteD[i][j].cor[k] = matriz[i + tam][j + tam].cor[k];
            }
        }
    }
}


rgb **algoritmoStrassen (int ***matrizPixel, int ***matrizFiltro, int tam, int valorCor) {
    rgb **matrizRes = alocaMatriz(tam);

    // Caso base:
    // Como avisado no trabalho, seria mais eficiente em matrizes de 32x32 ou menore serem resolvidas pela multiplicação tradicional.
    if (tam < 32) {
        multiplicaMatriz(matrizPixel, matrizFiltro, matrizRes, tam, valorCor);
        return matrizRes;
    }

    int novoTamanho = (tam/2);

    rgb **quadranteA = alocaMatriz(novoTamanho);
    rgb **quadranteB = alocaMatriz(novoTamanho);
    rgb **quadranteC = alocaMatriz(novoTamanho);
    rgb **quadranteD = alocaMatriz(novoTamanho);
    divideMatrizStrassen(matrizPixel, quadranteA, quadranteB, quadranteC, quadranteD, novoTamanho, valorCor);

    rgb **quadranteE = alocaMatriz(novoTamanho);
    rgb **quadranteF = alocaMatriz(novoTamanho);
    rgb **quadranteG = alocaMatriz(novoTamanho);
    rgb **quadranteH = alocaMatriz(novoTamanho);
    divideMatrizStrassen(matrizFiltro, quadranteE, quadranteF, quadranteG, quadranteH, novoTamanho, valorCor);


    rgb **P1, **P2, **P3, **P4, **P5, **P6, **P7;

    P1 = algoritmoStrassen(quadranteA, subtraiMatriz(quadranteF, quadranteH, novoTamanho, valorCor), novoTamanho, valorCor);
    P2 = algoritmoStrassen(somaMatriz(quadranteA, quadranteB, novoTamanho, valorCor), quadranteH, novoTamanho, valorCor);
    P3 = algoritmoStrassen(somaMatriz(quadranteC, quadranteD, novoTamanho, valorCor), quadranteE, novoTamanho, valorCor);
    P4 = algoritmoStrassen(quadranteD, subtraiMatriz(quadranteG, quadranteE, novoTamanho, valorCor), novoTamanho, valorCor);
    P5 = algoritmoStrassen(somaMatriz(quadranteA, quadranteD, novoTamanho, valorCor), somaMatriz(quadranteE, quadranteH, novoTamanho, valorCor), novoTamanho, valorCor);
    P6 = algoritmoStrassen(subtraiMatriz(quadranteB, quadranteD, novoTamanho, valorCor), somaMatriz(quadranteG, quadranteH, novoTamanho, valorCor), novoTamanho, valorCor);
    P7 = algoritmoStrassen(subtraiMatriz(quadranteA, quadranteC, novoTamanho, valorCor), somaMatriz(quadranteE, quadranteF, novoTamanho, valorCor), novoTamanho, valorCor);


    for (int i = 0; i < novoTamanho; i++) {
        for (int j = 0; j < novoTamanho; j++) {
            for (int k = 0; k < valorCor; k++) {
                matrizRes[i][j].cor[k] = P5[i][j].cor[k] + P4[i][j].cor[k] - P2[i][j].cor[k] + P6[i][j].cor[k];
                matrizRes[i][j + novoTamanho].cor[k] = P1[i][j].cor[k] + P2[i][j].cor[k];
                matrizRes[i + novoTamanho][j].cor[k] = P3[i][j].cor[k] + P4[i][j].cor[k];
                matrizRes[i + novoTamanho][j + novoTamanho].cor[k] = P5[i][j].cor[k] + P1[i][j].cor[k] - P3[i][j].cor[k] - P7[i][j].cor[k];

            }
        }
    }



    liberaMatriz(quadranteA, novoTamanho);
    liberaMatriz(quadranteB, novoTamanho);
    liberaMatriz(quadranteC, novoTamanho);
    liberaMatriz(quadranteD, novoTamanho);
    liberaMatriz(quadranteE, novoTamanho);
    liberaMatriz(quadranteF, novoTamanho);
    liberaMatriz(quadranteG, novoTamanho);
    liberaMatriz(quadranteH, novoTamanho);

    liberaMatriz(P1, novoTamanho);
    liberaMatriz(P2, novoTamanho);
    liberaMatriz(P3, novoTamanho);
    liberaMatriz(P4, novoTamanho);
    liberaMatriz(P5, novoTamanho);
    liberaMatriz(P6, novoTamanho);
    liberaMatriz(P7, novoTamanho);
    return matrizRes;
}



