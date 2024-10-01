#include <stdlib.h>
#include <stdio.h>


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

// Função para imprimir uma matriz RGB
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

void somaMatriz (int ***matrizA, int ***matrizB, int ***matrizResult, int tam, int rgb) {
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            for (int k = 0; k < rgb; k++) {
                matrizResult[i][j][k] = matrizA[i][j][k] + matrizB[i][j][k];
            }
        }
    }
}

void subtraiMatriz (int ***matrizA, int ***matrizB, int ***matrizResult, int tam, int rgb ) {
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            for (int k = 0; k < rgb; k++) {
                matrizResult[i][j][k] = matrizA[i][j][k] - matrizB[i][j][k];
            }
        }
    }
}

// Divide a matriz em quatro quadrantes (submatrizes)
void divideMatrizStrassen(int ***matrizOriginal, int ***quadranteA, int ***quadranteB, int ***quadranteC, int ***quadranteD, int tam, int rgb) {
    //  Formato depois da divisão:
    //  | A   B |
    //  | C   D |
    //  Cada quadrante copia os elementos de sua determinada região
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            for (int k = 0; k < rgb; k++) {
                quadranteA[i][j][k] = matrizOriginal[i][j][k];
                quadranteB[i][j][k] = matrizOriginal[i][j + tam][k];
                quadranteC[i][j][k] = matrizOriginal[i + tam][j][k];
                quadranteD[i][j][k] = matrizOriginal[i + tam][j + tam][k];
            }
        }
    }
}

int ***algoritmoStrassen (int ***matrizA, int ***matrizB, int tam, int rgb) {
    int ***matrizResult = alocaMatriz(tam, rgb);
    int novoTamanho = (tam/2);

    // Caso base:
    // Como avisado no trabalho, seria mais eficiente em matrizes de 32x32 ou menore serem resolvidas pela multiplicação tradicional.
    if (tam < 32) {
        multiplicaMatriz(matrizA, matrizB, matrizResult, tam, rgb);
    }
    else {
        int ***quadranteA = alocaMatriz(tam, rgb);
        int ***quadranteB = alocaMatriz(tam, rgb);
        int ***quadranteC = alocaMatriz(tam, rgb);
        int ***quadranteD = alocaMatriz(tam, rgb);
        divideMatrizStrassen(matrizA, quadranteA, quadranteB, quadranteC, quadranteD, novoTamanho, rgb);

        int ***quadranteE = alocaMatriz(tam, rgb);
        int ***quadranteF = alocaMatriz(tam, rgb);
        int ***quadranteG = alocaMatriz(tam, rgb);
        int ***quadranteH = alocaMatriz(tam, rgb);
        divideMatrizStrassen(matrizB, quadranteA, quadranteB, quadranteC, quadranteD, novoTamanho, rgb);


        int ***matrizTemp = alocaMatriz(novoTamanho, rgb);
        int ***matrizTemp2 = alocaMatriz(novoTamanho, rgb);

        int ***P1, ***P2, ***P3, ***P4, ***P5, ***P6, ***P7;

        // Produtos de Strassen
        //P1 = A.(F-H)
        subtraiMatriz(quadranteF, quadranteH, matrizTemp, novoTamanho, rgb);
        P1 = algoritmoStrassen(quadranteA, matrizTemp, novoTamanho, rgb);
        //P2 = (A+B).H
        somaMatriz(quadranteA, quadranteB, matrizTemp, novoTamanho, rgb);
        P2 = algoritmoStrassen(quadranteH, matrizTemp, novoTamanho, rgb);
        //P3 = (C+D).E
        somaMatriz(quadranteC,quadranteA, matrizTemp, novoTamanho, rgb);
        P3 = algoritmoStrassen(quadranteE, matrizTemp, novoTamanho, rgb);
        //P4 = D.(G-E)
        subtraiMatriz(quadranteG, quadranteA, matrizTemp, novoTamanho, rgb);
        P4 = algoritmoStrassen(quadranteD, matrizTemp, novoTamanho, rgb);
        //P5 = (A+D).(E+H)
        somaMatriz(quadranteA, quadranteD, matrizTemp, novoTamanho, rgb);
        somaMatriz(quadranteE, quadranteH, matrizTemp2, novoTamanho, rgb);
        P5 = algoritmoStrassen(matrizTemp, matrizTemp2, novoTamanho, rgb);
        //P6 = (B-D).(G+H)
        subtraiMatriz(quadranteB, quadranteD, matrizTemp, novoTamanho, rgb);
        somaMatriz(quadranteG, quadranteH, matrizTemp2, novoTamanho, rgb);
        P6 = algoritmoStrassen(matrizTemp, matrizTemp2, novoTamanho, rgb);
        //P7 = (A-C).(E-F)
        subtraiMatriz(quadranteA, quadranteC, matrizTemp, novoTamanho, rgb);
        subtraiMatriz(quadranteE, quadranteF, matrizTemp2, novoTamanho, rgb);
        P7 = algoritmoStrassen(matrizTemp, matrizTemp2, novoTamanho, rgb);

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

        for (int i = 0; i < novoTamanho; i++) {
            for (int j = 0; j < novoTamanho; j++) {
                for (int k = 0; k < rgb; k++) {
                    matrizResult[i][j][k] = P5[i][j][k] + P4[i][j][k] - P2[i][j][k] + P6[i][j][k];
                    matrizResult[i][j + novoTamanho][k] = P1[i][j][k] + P2[i][j][k];
                    matrizResult[i + novoTamanho][j][k] = P3[i][j][k] + P4[i][j][k];
                    matrizResult[i + novoTamanho][j + novoTamanho][k] = P5[i][j][k] + P1[i][j][k] - P3[i][j][k] - P7[i][j][k];

                }
            }
        }

        liberaMatriz(matrizTemp, novoTamanho);
        liberaMatriz(matrizTemp2, novoTamanho);

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
    }

    return matrizResult;
}



//
// // Função para dividir uma matriz em quatro submatrizes
// void dividirMatriz(int n, int*** mat, int*** matrizDividida, int iInicio, int jInicio, int rgbSize) {
//     int novoTamanho = n / 2;
//     for (int i = 0, i2 = iInicio; i < novoTamanho; i++, i2++) {
//         for (int j = 0, j2 = jInicio; j < novoTamanho; j++, j2++) {
//             for (int k = 0; k < rgbSize; k++) {
//                 matrizDividida[i][j][k] = mat[i2][j2][k];
//             }
//         }
//     }
// }
//
// // Função para juntar submatrizes em uma matriz maior
// void juntarMatriz(int n, int*** matDividida, int*** matrizCompleta, int iInicio, int jInicio, int rgbSize) {
//     int novoTamanho = n / 2;
//     for (int i = 0, i2 = iInicio; i < novoTamanho; i++, i2++) {
//         for (int j = 0, j2 = jInicio; j < novoTamanho; j++, j2++) {
//             for (int k = 0; k < rgbSize; k++) {
//                 matrizCompleta[i2][j2][k] = matDividida[i][j][k];
//             }
//         }
//     }
// }
//
// // Função para somar duas matrizes
// void adicaoMatrizes(int n, int*** matA, int*** matB, int*** matResultante, int rgbSize) {
//     for (int i = 0; i < n; i++) {
//         for (int j = 0; j < n; j++) {
//             for (int k = 0; k < rgbSize; k++) {
//                 matResultante[i][j][k] = matA[i][j][k] + matB[i][j][k];
//             }
//         }
//     }
// }
//
// // Função para subtrair duas matrizes
// void subtracaoMatrizes(int n, int*** matA, int*** matB, int*** matResultante, int rgbSize) {
//     for (int i = 0; i < n; i++) {
//         for (int j = 0; j < n; j++) {
//             for (int k = 0; k < rgbSize; k++) {
//                 matResultante[i][j][k] = matA[i][j][k] - matB[i][j][k];
//             }
//         }
//     }
// }
//
// // Função principal que implementa a multiplicação de Strassen para matrizes RGB
// int*** multiplicacaoMatrizesStrassen(int*** matA, int*** matB, int n, int rgbSize) {
//     int*** resultado = alocaMatriz(n, rgbSize);  // Aloca matriz para o resultado
//     int tamanhoMatrizDividido = n / 2;
//
//     if (n < 32) {
//         // Usa multiplicação clássica para matrizes pequenas
//         return multiplicaMatriz(matA, matB, resultado, n, rgbSize);
//     } else {
//         // Divide a matriz A em 4 partes
//         int*** matAParte1 = alocaMatriz(tamanhoMatrizDividido, rgbSize);
//         dividirMatriz(n, matA, matAParte1, 0, 0, rgbSize);
//
//         int*** matAParte2 = alocaMatriz(tamanhoMatrizDividido, rgbSize);
//         dividirMatriz(n, matA, matAParte2, 0, tamanhoMatrizDividido, rgbSize);
//
//         int*** matAParte3 = alocaMatriz(tamanhoMatrizDividido, rgbSize);
//         dividirMatriz(n, matA, matAParte3, tamanhoMatrizDividido, 0, rgbSize);
//
//         int*** matAParte4 = alocaMatriz(tamanhoMatrizDividido, rgbSize);
//         dividirMatriz(n, matA, matAParte4, tamanhoMatrizDividido, tamanhoMatrizDividido, rgbSize);
//
//         // Divide a matriz B em 4 partes
//         int*** matBParte1 = alocaMatriz(tamanhoMatrizDividido, rgbSize);
//         dividirMatriz(n, matB, matBParte1, 0, 0, rgbSize);
//
//         int*** matBParte2 = alocaMatriz(tamanhoMatrizDividido, rgbSize);
//         dividirMatriz(n, matB, matBParte2, 0, tamanhoMatrizDividido, rgbSize);
//
//         int*** matBParte3 = alocaMatriz(tamanhoMatrizDividido, rgbSize);
//         dividirMatriz(n, matB, matBParte3, tamanhoMatrizDividido, 0, rgbSize);
//
//         int*** matBParte4 = alocaMatriz(tamanhoMatrizDividido, rgbSize);
//         dividirMatriz(n, matB, matBParte4, tamanhoMatrizDividido, tamanhoMatrizDividido, rgbSize);
//
//         // Passos do algoritmo de Strassen (P1 a P7)
//         int*** temp1 = alocaMatriz(tamanhoMatrizDividido, rgbSize);
//         int*** temp2 = alocaMatriz(tamanhoMatrizDividido, rgbSize);
//
//         // P1 = A.(F-H)
//         subtracaoMatrizes(tamanhoMatrizDividido, matBParte2, matBParte4, temp1, rgbSize);
//         int*** p1 = multiplicacaoMatrizesStrassen(matAParte1, temp1, tamanhoMatrizDividido, rgbSize);
//
//         // P2 = (A+B).H
//         adicaoMatrizes(tamanhoMatrizDividido, matAParte1, matAParte2, temp1, rgbSize);
//         int*** p2 = multiplicacaoMatrizesStrassen(temp1, matBParte4, tamanhoMatrizDividido, rgbSize);
//
//         // P3 = (C+D).E
//         adicaoMatrizes(tamanhoMatrizDividido, matAParte3, matAParte4, temp1, rgbSize);
//         int*** p3 = multiplicacaoMatrizesStrassen(temp1, matBParte1, tamanhoMatrizDividido, rgbSize);
//
//         // P4 = D.(G-E)
//         subtracaoMatrizes(tamanhoMatrizDividido, matBParte3, matBParte1, temp1, rgbSize);
//         int*** p4 = multiplicacaoMatrizesStrassen(matAParte4, temp1, tamanhoMatrizDividido, rgbSize);
//
//         // P5 = (A+D).(E+H)
//         adicaoMatrizes(tamanhoMatrizDividido, matAParte1, matAParte4, temp1, rgbSize);
//         adicaoMatrizes(tamanhoMatrizDividido, matBParte1, matBParte4, temp2, rgbSize);
//         int*** p5 = multiplicacaoMatrizesStrassen(temp1, temp2, tamanhoMatrizDividido, rgbSize);
//
//         // P6 = (B-D).(G+H)
//         subtracaoMatrizes(tamanhoMatrizDividido, matAParte2, matAParte4, temp1, rgbSize);
//         adicaoMatrizes(tamanhoMatrizDividido, matBParte3, matBParte4, temp2, rgbSize);
//         int*** p6 = multiplicacaoMatrizesStrassen(temp1, temp2, tamanhoMatrizDividido, rgbSize);
//
//         // P7 = (A-C).(E+F)
//         subtracaoMatrizes(tamanhoMatrizDividido, matAParte1, matAParte3, temp1, rgbSize);
//         adicaoMatrizes(tamanhoMatrizDividido, matBParte1, matBParte2, temp2, rgbSize);
//         int*** p7 = multiplicacaoMatrizesStrassen(temp1, temp2, tamanhoMatrizDividido, rgbSize);
//
//         // Calcula os quadrantes da matriz resultado
//         int*** matResultanteP1 = alocaMatriz(tamanhoMatrizDividido, rgbSize);
//         adicaoMatrizes(tamanhoMatrizDividido, p5, p4, temp1, rgbSize);
//         subtracaoMatrizes(tamanhoMatrizDividido, temp1, p2, temp2, rgbSize);
//         adicaoMatrizes(tamanhoMatrizDividido, temp2, p6, matResultanteP1, rgbSize);
//
//         int*** matResultanteP2 = alocaMatriz(tamanhoMatrizDividido, rgbSize);
//         adicaoMatrizes(tamanhoMatrizDividido, p1, p2, matResultanteP2, rgbSize);
//
//         int*** matResultanteP3 = alocaMatriz(tamanhoMatrizDividido, rgbSize);
//         adicaoMatrizes(tamanhoMatrizDividido, p3, p4, matResultanteP3, rgbSize);
//
//         int*** matResultanteP4 = alocaMatriz(tamanhoMatrizDividido, rgbSize);
//         adicaoMatrizes(tamanhoMatrizDividido, p1, p5, temp1, rgbSize);
//         subtracaoMatrizes(tamanhoMatrizDividido, temp1, p3, temp2, rgbSize);
//         subtracaoMatrizes(tamanhoMatrizDividido, temp2, p7, matResultanteP4, rgbSize);
//
//         // Junta os quadrantes para formar a matriz completa
//         juntarMatriz(n, matResultanteP1, resultado, 0, 0, rgbSize);
//         juntarMatriz(n, matResultanteP2, resultado, 0, tamanhoMatrizDividido, rgbSize);
//         juntarMatriz(n, matResultanteP3, resultado, tamanhoMatrizDividido, 0, rgbSize);
//         juntarMatriz(n, matResultanteP4, resultado, tamanhoMatrizDividido, tamanhoMatrizDividido, rgbSize);
//
//         // Libera memória das submatrizes e temporárias
//         liberaMatriz(matAParte1, tamanhoMatrizDividido);
//         liberaMatriz(matAParte2, tamanhoMatrizDividido);
//         liberaMatriz(matAParte3, tamanhoMatrizDividido);
//         liberaMatriz(matAParte4, tamanhoMatrizDividido);
//         liberaMatriz(matBParte1, tamanhoMatrizDividido);
//         liberaMatriz(matBParte2, tamanhoMatrizDividido);
//         liberaMatriz(matBParte3, tamanhoMatrizDividido);
//         liberaMatriz(matBParte4, tamanhoMatrizDividido);
//         liberaMatriz(temp1, tamanhoMatrizDividido);
//         liberaMatriz(temp2, tamanhoMatrizDividido);
//         liberaMatriz(p1, tamanhoMatrizDividido);
//         liberaMatriz(p2, tamanhoMatrizDividido);
//         liberaMatriz(p3, tamanhoMatrizDividido);
//         liberaMatriz(p4, tamanhoMatrizDividido);
//         liberaMatriz(p5, tamanhoMatrizDividido);
//         liberaMatriz(p6, tamanhoMatrizDividido);
//         liberaMatriz(p7, tamanhoMatrizDividido);
//     }
//
//     return resultado;  // Retorna a matriz resultado
// }
