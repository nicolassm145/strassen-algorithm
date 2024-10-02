#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int ***alocaMatriz(int tam, int rgb);
void liberaMatriz(int ***matriz, int tam);
void leMatriz(FILE *file, int ***matriz, int tam, int rgb);
void imprimeMatriz(int ***matriz, int tam, int rgb);
int ***multiplicaMatriz(int ***matrizA, int ***matrizB, int ***matrizResult, int tam, int rgb);

//void juntarMatriz(int n, int*** matDividida, int*** matrizCompleta, int iInicio, int jInicio, int rgbSize);
int ***somaMatriz (int ***primeiraParte, int ***segundaParte, int tam, int rgb);
int ***subtraiMatriz (int ***primeiraParte, int ***segundaParte, int tam, int rgb);
void divideMatrizStrassen(int ***matriz, int ***quadranteA, int ***quadranteB, int ***quadranteC, int ***quadranteD, int tam, int rgb);
int ***algoritmoStrassen (int ***matrizPixel, int ***matrizFiltro, int tam, int rgb);


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
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\n\nTempo: %f segundos\n", cpu_time_used);
    imprimeMatriz(matrizResult, n, rgb);




    liberaMatriz(matrizA, n);
    liberaMatriz(matrizB, n);
    liberaMatriz(matrizResult, n);

    fclose(file);

    return 0;
}


int ***alocaMatriz(int tam, int rgb) {
    int ***matriz = malloc(sizeof(int**) * tam);
    for (int i = 0; i < tam; i++) {
        matriz[i] = malloc(sizeof(int*) * tam);
        for (int j = 0; j < tam; j++) {
            matriz[i][j] = malloc(sizeof(int) * rgb);
        }
    }
    return matriz;
}

void liberaMatriz(int ***matriz, int tam) {
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            free(matriz[i][j]);
        }
        free(matriz[i]);
    }
    free(matriz);
}

void leMatriz(FILE *file, int ***matriz, int tam, int rgb) {
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            for (int k = 0; k < rgb; k++) {
                fscanf(file, "%d", &matriz[i][j][k]);
            }
        }
    }
}


void imprimeMatriz(int ***matriz, int tam, int rgb) {
    for (int i = 0; i < tam; i++) {
        printf("\n");
        for (int j = 0; j < tam; j++) {
            for (int k = 0; k < rgb; k++) {
                printf("%d ", matriz[i][j][k]);
            }
        }
    }
}

// Função para multiplicação clássica de matrizes RGB
int ***multiplicaMatriz(int ***matrizA, int ***matrizB, int ***matrizResult, int tam, int rgb) {
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            for (int k = 0; k < rgb; k++) {
                matrizResult[i][j][k] = 0;  // Inicializa matriz resultado
            }
            for (int l = 0; l < tam; l++) {
                for (int k = 0; k < rgb; k++) {
                    matrizResult[i][j][k] += matrizA[i][l][k] * matrizB[l][j][k];  // Multiplicação de cada componente RGB
                }
            }
        }
    }
    return matrizResult;
}

int ***somaMatriz (int ***primeiraParte, int ***segundaParte, int tam, int rgb) {
    int ***result = alocaMatriz(tam, rgb);
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            for (int k = 0; k < rgb; k++) {
                result[i][j][k] = primeiraParte[i][j][k] + segundaParte[i][j][k];
            }
        }
    }
    return result;
}

int ***subtraiMatriz (int ***primeiraParte, int ***segundaParte, int tam, int rgb) {
    int ***result = alocaMatriz(tam, rgb);
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            for (int k = 0; k < rgb; k++) {
                result[i][j][k] = primeiraParte[i][j][k] - segundaParte[i][j][k];
            }
        }
    }
    return result;
}

// Divide a matriz em quatro quadrantes (submatrizes)
void divideMatrizStrassen(int ***matriz, int ***quadranteA, int ***quadranteB, int ***quadranteC, int ***quadranteD, int tam, int rgb) {
    //  Formato depois da divisão:
    //  | A   B |
    //  | C   D |
    //  Cada quadrante copia os elementos de sua determinada região
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            for (int k = 0; k < rgb; k++) {
                quadranteA[i][j][k] = matriz[i][j][k];
                quadranteB[i][j][k] = matriz[i][j + tam][k];
                quadranteC[i][j][k] = matriz[i + tam][j][k];
                quadranteD[i][j][k] = matriz[i + tam][j + tam][k];
            }
        }
    }
}


int ***algoritmoStrassen (int ***matrizPixel, int ***matrizFiltro, int tam, int rgb) {
    int ***matrizRes = alocaMatriz(tam, rgb);


    // Caso base:
    // Como avisado no trabalho, seria mais eficiente em matrizes de 32x32 ou menore serem resolvidas pela multiplicação tradicional.
    // if (tam < 32) {
    //     multiplicaMatriz(matrizPixel, matrizFiltro, matrizRes, tam, rgb);
    // }
    if (tam == 1) {
        for (int i = 0; i < rgb; i++){
        matrizRes[0][0][i] = matrizPixel[0][0][i] * matrizFiltro[0][0][i];
    }
        return matrizRes;
    }

    int novoTamanho = (tam/2);

        int ***quadranteA = alocaMatriz(novoTamanho, rgb);
        int ***quadranteB = alocaMatriz(novoTamanho, rgb);
        int ***quadranteC = alocaMatriz(novoTamanho, rgb);
        int ***quadranteD = alocaMatriz(novoTamanho, rgb);
        divideMatrizStrassen(matrizPixel, quadranteA, quadranteB, quadranteC, quadranteD, novoTamanho, rgb);

        int ***quadranteE = alocaMatriz(novoTamanho, rgb);
        int ***quadranteF = alocaMatriz(novoTamanho, rgb);
        int ***quadranteG = alocaMatriz(novoTamanho, rgb);
        int ***quadranteH = alocaMatriz(novoTamanho, rgb);
        divideMatrizStrassen(matrizFiltro, quadranteE, quadranteF, quadranteG, quadranteH, novoTamanho, rgb);


        // int ***matrizTemp = alocaMatriz(novoTamanho, rgb);
        // int ***matrizTemp2 = alocaMatriz(novoTamanho, rgb);

        int ***P1, ***P2, ***P3, ***P4, ***P5, ***P6, ***P7;

        // Produtos de Strassen
        // //P1 = A.(F-H)
        // subtraiMatriz(quadranteF, quadranteH, novoTamanho, rgb);
        // P1 = algoritmoStrassen(quadranteA, matrizTemp, novoTamanho, rgb);
        // //P2 = (A+B).H
        // somaMatriz(quadranteA, quadranteB, novoTamanho, rgb);
        // P2 = algoritmoStrassen(quadranteH, matrizTemp, novoTamanho, rgb);
        // //P3 = (C+D).E
        // somaMatriz(quadranteC,quadranteA, novoTamanho, rgb);
        // P3 = algoritmoStrassen(quadranteE, matrizTemp, novoTamanho, rgb);
        // //P4 = D.(G-E)
        // subtraiMatriz(quadranteG, quadranteA, novoTamanho, rgb);
        // P4 = algoritmoStrassen(quadranteD, matrizTemp, novoTamanho, rgb);
        // //P5 = (A+D).(E+H)
        // somaMatriz(quadranteA, quadranteD, novoTamanho, rgb);
        // somaMatriz(quadranteE, quadranteH, novoTamanho, rgb);
        // P5 = algoritmoStrassen(matrizTemp, matrizTemp2, novoTamanho, rgb);
        // //P6 = (B-D).(G+H)
        // subtraiMatriz(quadranteB, quadranteD, novoTamanho, rgb);
        // somaMatriz(quadranteG, quadranteH, novoTamanho, rgb);
        // P6 = algoritmoStrassen(matrizTemp, matrizTemp2, novoTamanho, rgb);
        // //P7 = (A-C).(E-F)
        // subtraiMatriz(quadranteA, quadranteC, novoTamanho, rgb);
        // subtraiMatriz(quadranteE, quadranteF, novoTamanho, rgb);
        // P7 = algoritmoStrassen(matrizTemp, matrizTemp2, novoTamanho, rgb);

    P1 = algoritmoStrassen(quadranteA, subtraiMatriz(quadranteF, quadranteH, novoTamanho, rgb), novoTamanho, rgb);
    P2 = algoritmoStrassen(somaMatriz(quadranteA, quadranteB, novoTamanho, rgb), quadranteH, novoTamanho, rgb);
    P3 = algoritmoStrassen(somaMatriz(quadranteC, quadranteD, novoTamanho, rgb), quadranteE, novoTamanho, rgb);
    P4 = algoritmoStrassen(quadranteD, subtraiMatriz(quadranteG, quadranteE, novoTamanho, rgb), novoTamanho, rgb);
    P5 = algoritmoStrassen(somaMatriz(quadranteA, quadranteD, novoTamanho, rgb), somaMatriz(quadranteE, quadranteH, novoTamanho, rgb), novoTamanho, rgb);
    P6 = algoritmoStrassen(subtraiMatriz(quadranteB, quadranteD, novoTamanho, rgb), somaMatriz(quadranteG, quadranteH, novoTamanho, rgb), novoTamanho, rgb);
    P7 = algoritmoStrassen(subtraiMatriz(quadranteA, quadranteC, novoTamanho, rgb), somaMatriz(quadranteE, quadranteF, novoTamanho, rgb), novoTamanho, rgb);


    for (int i = 0; i < novoTamanho; i++) {
        for (int j = 0; j < novoTamanho; j++) {
            for (int k = 0; k < rgb; k++) {
                matrizRes[i][j][k] = P5[i][j][k] + P4[i][j][k] - P2[i][j][k] + P6[i][j][k];
                matrizRes[i][j + novoTamanho][k] = P1[i][j][k] + P2[i][j][k];
                matrizRes[i + novoTamanho][j][k] = P3[i][j][k] + P4[i][j][k];
                matrizRes[i + novoTamanho][j + novoTamanho][k] = P5[i][j][k] + P1[i][j][k] - P3[i][j][k] - P7[i][j][k];

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

        // // Matriz A * Matriz B:
        // // São quatro quadrantes
        // int ***primeiroQuadrante = alocaMatriz(novoTamanho, rgb);
        // int ***segundoQuadrante = alocaMatriz(novoTamanho, rgb);
        // int ***terceiroQuadrante = alocaMatriz(novoTamanho, rgb);
        // int ***quartoQuadrante = alocaMatriz(novoTamanho, rgb);

        // // Primeiro quadrante = P5 + P4 - P2 + P6
        // somaMatriz(P5, P4, matrizTemp, novoTamanho, rgb);
        // subtraiMatriz(matrizTemp, P2, matrizTemp, novoTamanho, rgb);
        // somaMatriz(matrizTemp2, P6, primeiroQuadrante, novoTamanho, rgb);
        //
        // // Segundo quadrante = P1 + P2
        // somaMatriz(P1, P2, segundoQuadrante, novoTamanho, rgb);
        // // Terceiro quadrante = P3 + P4
        // somaMatriz(P3, P4, terceiroQuadrante, novoTamanho, rgb);
        //
        // // Quarto quadrante = P1 + P5 − P3 − P7
        // somaMatriz(P1, P5, matrizTemp, novoTamanho, rgb);
        // subtraiMatriz(matrizTemp, P3, matrizTemp, novoTamanho, rgb);
        // subtraiMatriz(matrizTemp2, P7, quartoQuadrante, novoTamanho, rgb);


        // liberaMatriz(matrizTemp, novoTamanho);
        // liberaMatriz(matrizTemp2, novoTamanho);

        liberaMatriz(P1, novoTamanho);
        liberaMatriz(P2, novoTamanho);
        liberaMatriz(P3, novoTamanho);
        liberaMatriz(P4, novoTamanho);
        liberaMatriz(P5, novoTamanho);
        liberaMatriz(P6, novoTamanho);
        liberaMatriz(P7, novoTamanho);



        // liberaMatriz(primeiroQuadrante, novoTamanho);
        // liberaMatriz(segundoQuadrante, novoTamanho);
        // liberaMatriz(terceiroQuadrante, novoTamanho);
        // liberaMatriz(quartoQuadrante, novoTamanho);


    return matrizRes;
}