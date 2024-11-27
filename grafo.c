#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define INF INT_MAX
#define MAX_VERTICES 1000

int graph[MAX_VERTICES][MAX_VERTICES];
int VERTICES;

void criarGrafoAleatorio(int num_vertices, double probabilidade_arestas, int peso_max) {
    VERTICES = num_vertices;

    // Inicializa a matriz de adjacencia
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            graph[i][j] = (i == j) ? 0 : INF;
        }
    }

    srand(time(NULL));
    for (int i = 0; i < num_vertices; i++) {
        for (int j = i + 1; j < num_vertices; j++) {
            double random_value = (double)rand() / RAND_MAX;
            if (random_value < probabilidade_arestas) {
                int peso = rand() % peso_max + 1;
                graph[i][j] = peso;
                graph[j][i] = peso; // Grafo nao direcionado
            }
        }
    }

    // Exporta o grafo para um arquivo texto
    FILE *arquivo = fopen("grafo.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo grafo.txt\n");
        exit(1);
    }

    for (int i = 0; i < num_vertices; i++) {
        for (int j = i + 1; j < num_vertices; j++) {
            if (graph[i][j] != INF) {
                fprintf(arquivo, "%d %d %d\n", i, j, graph[i][j]);
            }
        }
    }

    fclose(arquivo);
    printf("Grafo exportado para grafo.txt\n");
}

int main() {
    int num_vertices = 1000;
    double probabilidade_arestas = 0.2;
    int peso_max = 10;

    criarGrafoAleatorio(num_vertices, probabilidade_arestas, peso_max);

    return 0;
}
