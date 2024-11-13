#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF INT_MAX
#define MAX_VERTICES 100

int graph[MAX_VERTICES][MAX_VERTICES];
int VERTICES;

void carregarGrafo(char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
        exit(1);
    }

    // Inicializa a matriz de adjacencia
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            graph[i][j] = (i == j) ? 0 : INF;
        }
    }

    int u, v, peso;
    VERTICES = 0;

    // Le cada linha do arquivo e preenche a matriz de adjacencia
    while (fscanf(arquivo, "%d %d %d", &u, &v, &peso) != EOF) {
        graph[u][v] = peso;
        graph[v][u] = peso;  // Grafo não-direcionado
        if (u > VERTICES) VERTICES = u;
        if (v > VERTICES) VERTICES = v;
    }
    VERTICES++;

    fclose(arquivo);
}

void dijkstra(int start, int dist[]) {
    int visited[MAX_VERTICES] = {0};

    // Inicializa distancias com infinito e a distancia do vertice inicial com 0
    for (int i = 0; i < VERTICES; i++) {
        dist[i] = INF;
    }
    dist[start] = 0;

    // Algoritmo de Dijkstra
    for (int count = 0; count < VERTICES - 1; count++) {
        int u = -1;
        int min_dist = INF;

        for (int i = 0; i < VERTICES; i++) {
            if (!visited[i] && dist[i] < min_dist) {
                u = i;
                min_dist = dist[i];
            }
        }

        if (u == -1) break;

        visited[u] = 1;

        for (int v = 0; v < VERTICES; v++) {
            if (!visited[v] && graph[u][v] != INF && dist[u] != INF && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }
}

void exportarCaminhosMaisCurtos(char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s para escrita\n", nome_arquivo);
        exit(1);
    }

    int dist[MAX_VERTICES];
    
    for (int i = 0; i < VERTICES; i++) {
        dijkstra(i, dist);
        fprintf(arquivo, "Distancias a partir do vertice %d:\n", i);
        for (int j = 0; j < VERTICES; j++) {
            if (dist[j] == INF) {
                fprintf(arquivo, "Vertice %d: INFINITO\n", j);
            } else {
                fprintf(arquivo, "Vertice %d: %d\n", j, dist[j]);
            }
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
}

int main() {
    carregarGrafo("grafo.txt");
    exportarCaminhosMaisCurtos("resultado-sequencial.txt");

    printf("Caminhos mais curtos exportados para o arquivo resultado-sequencial.txt\n");

    return 0;
}
