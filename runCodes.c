#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int ***alocaMatriz(int tam, int rgb);
void liberaMatriz(int ***matriz, int tam);
void leMatriz(FILE *file, int ***matriz, int tam, int rgb);
void imprimeMatriz(int ***matriz, int tam, int rgb);
int ***multiplicaMatriz(int ***matrizA, int ***matrizB, int ***matrizResult, int tam, int rgb);
void somaMatriz (int ***matrizA, int ***matrizB, int ***matrizResult, int tam, int rgb);
void subtraiMatriz (int ***matrizA, int ***matrizB, int ***matrizResult, int tam, int rgb);
void divideMatrizStrassen(int ***matrizOriginal, int ***quadranteA, int ***quadranteB, int ***quadranteC, int ***quadranteD, int tam, int rgb);
int ***algoritmoStrassen (int ***matrizA, int ***matrizB, int tam, int rgb);


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

void subtraiMatriz (int ***matrizA, int ***matrizB, int ***matrizResult, int tam, int rgb) {
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
