
int ***alocaMatriz(int n, int rgb) {
    int ***matriz = malloc(sizeof(int**) * n);

    for (int i = 0; i < n; i++) {
        matriz[i] = malloc(sizeof(int*) * n);
        for (int j = 0; j < n; j++)
            matriz[i][j] = malloc(sizeof(int) * n);
    }

    return matriz;
}

void liberaMatriz(int ***matriz, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            free(matriz[i][j]);
    }
    free(matriz);
}

void leMatriz(FILE *file, int ***matriz, int n, int rgb) {

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < rgb; k++)
                fscanf(file, "%d", &matriz[i][j][k]);
        }
    }
}

void imprimeMatriz(int ***matriz, int n, int rgb) {
    for (int i = 0; i < n; i++) {
        printf("\n");
        for (int j = 0; j < n; j++) {
            printf("   ");
            for (int k = 0; k < rgb; k++)
                printf("%d ", matriz[i][j][k]);
        }
    }
}