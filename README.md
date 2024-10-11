---

# Multiplicação de Matrizes (Strassen's Algorithm)

Este projeto implementa o **Algoritmo de Strassen** para multiplicação eficiente de matrizes, com foco na aplicação prática de filtros e transformações em imagens no formato **PPM (P3)**. Ele foi desenvolvido como parte do **Trabalho 01** da disciplina **CTCO04 - Projeto e Análise de Algoritmos**, e tem como objetivo explorar técnicas avançadas de multiplicação de matrizes utilizando o conceito de divisão e conquista.

## Objetivos

- Implementar o **Algoritmo de Strassen** para multiplicação de matrizes quadradas.
- Utilizar o algoritmo para aplicar filtros e transformações em imagens de formato PPM (P3), onde as cores são representadas por valores RGB.
- Comparar a eficiência do algoritmo de Strassen com a multiplicação clássica de matrizes, especialmente para matrizes de grandes dimensões.
- Demonstrar o uso de algoritmos de divisão e conquista em um cenário de manipulação de imagens, aplicando operações sobre matrizes de pixels.

## Algoritmo de Strassen

O algoritmo de Strassen é uma técnica eficiente para multiplicação de matrizes que reduz a complexidade temporal comparada à multiplicação tradicional. Ele utiliza recursão e o método de divisão e conquista para dividir matrizes grandes em submatrizes menores, realizando operações específicas que diminuem o número de multiplicações necessárias.

No projeto, o algoritmo foi implementado de forma a ser aplicado a matrizes RGB, onde cada elemento da matriz é um vetor de três componentes (vermelho, verde, azul). A solução também inclui um limite para o uso de Strassen, onde matrizes pequenas (por exemplo, `n < 64`) são multiplicadas pela abordagem clássica, melhorando a eficiência geral.

## Formato PPM (P3)

O formato **PPM (P3)** é utilizado para armazenar e manipular imagens. Ele representa cada pixel por três valores inteiros, correspondentes às intensidades das cores vermelho (R), verde (G), e azul (B), variando de 0 a 255. Neste projeto, tanto a matriz de imagem quanto a matriz de filtro são fornecidas no formato PPM.

Exemplo de uma imagem no formato PPM:
```
P3
4 4
255
255 0 0  255 0 0  0 255 0  0 255 0
255 0 0  255 0 0  0 255 0  0 255 0
0 0 255  0 0 255  0 255 255  0 255 255
0 0 255  0 0 255  0 255 255  0 255 255
```

## Estrutura do Projeto

- **Leitura de Matriz PPM**: O programa lê uma imagem no formato PPM e a converte para uma matriz de pixels RGB.
- **Aplicação de Filtros**: Um filtro de mesma dimensão que a imagem é lido e aplicado pixel a pixel, multiplicando as matrizes por componentes RGB.
- **Multiplicação de Matrizes (Clássica e Strassen)**: Dependendo do tamanho da matriz, o algoritmo aplica a multiplicação clássica ou o de Strassen.
- **Saída no Formato PPM**: Após a multiplicação, o programa imprime a imagem resultante no formato PPM.

## Execução

O programa deve ser executado lendo a imagem e o filtro da entrada padrão, ambos no formato PPM. Após o processamento, a imagem resultante é impressa na saída padrão.

## Considerações Finais

Este projeto demonstra como algoritmos avançados de multiplicação de matrizes podem ser utilizados em operações práticas, como manipulação de imagens, aplicando filtros e transformações eficientes. A implementação utiliza o algoritmo de Strassen para otimizar a multiplicação de matrizes grandes, respeitando as limitações de tempo de execução especificadas na disciplina.

---
