#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file;
    char filepath[100];
    int largura, altura, maxCor;
    char formato[3];

    snprintf(filepath, sizeof(filepath), "../test cases/1.in");
    file = fopen(filepath, "r");


    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return EXIT_FAILURE;
    }

    // Leitura do cabeçalho da imagem PPM (formato, largura, altura e escala de cor)
    fscanf(file, "%s", formato);
    if (strcmp(formato, "P3") != 0) {
        printf("Formato de arquivo não suportado.\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    fscanf(file, "%d %d", &largura, &altura);
    fscanf(file, "%d", &maxCor);

    printf("Formato: %s\n", formato);
    printf("Largura: %d, Altura: %d, Máx Cor: %d\n", largura, altura, maxCor);
    fclose(file);


}
