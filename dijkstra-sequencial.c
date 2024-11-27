#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define INF INT_MAX
#define MAX_VERTICES 1000

int graph[MAX_VERTICES][MAX_VERTICES];
int VERTICES;

// Função para ler o grafo de um arquivo
void lerGrafo(char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
        exit(1);
    }

    // Inicializa a matriz de adjacência
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            graph[i][j] = (i == j) ? 0 : INF;
        }
    }

    int u, v, peso;
    while (fscanf(arquivo, "%d %d %d", &u, &v, &peso) != EOF) {
        graph[u][v] = peso;
        graph[v][u] = peso; // Grafo não direcionado
    }

    fclose(arquivo);

    // Determina o número de vértices a partir do maior vértice lido
    VERTICES = 0;
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            if (graph[i][j] != INF) {
                if (i > VERTICES) VERTICES = i;
                if (j > VERTICES) VERTICES = j;
            }
        }
    }
    VERTICES++; // Ajusta para o número total de vértices
}

// Função para aplicar o algoritmo de Dijkstra
void dijkstra(int start, int dist[]) {
    int visited[MAX_VERTICES] = {0};

    // Inicializa distâncias com infinito e a distância do vértice inicial como 0
    for (int i = 0; i < VERTICES; i++) {
        dist[i] = INF;
    }
    dist[start] = 0;

    // Algoritmo de Dijkstra
    for (int count = 0; count < VERTICES - 1; count++) {
        int u = -1;
        int min_dist = INF;

        // Encontra o vértice com a menor distância
        for (int i = 0; i < VERTICES; i++) {
            if (!visited[i] && dist[i] < min_dist) {
                u = i;
                min_dist = dist[i];
            }
        }

        if (u == -1) break;

        visited[u] = 1;

        // Atualiza distâncias dos vizinhos de u
        for (int v = 0; v < VERTICES; v++) {
            if (!visited[v] && graph[u][v] != INF && dist[u] != INF && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }
}

// Função para exportar os caminhos mais curtos para um arquivo
void exportarCaminhosMaisCurtos(char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s para escrita\n", nome_arquivo);
        exit(1);
    }

    int dist[MAX_VERTICES];

    // Executa o algoritmo de Dijkstra para cada vértice
    for (int i = 0; i < VERTICES; i++) {
        dijkstra(i, dist);
        fprintf(arquivo, "Distâncias a partir do vértice %d:\n", i);
        for (int j = 0; j < VERTICES; j++) {
            if (dist[j] == INF) {
                fprintf(arquivo, "Vértice %d: INFINITO\n", j);
            } else {
                fprintf(arquivo, "Vértice %d: %d\n", j, dist[j]);
            }
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
}

int main() {
    clock_t start, end;
    double cpu_time_used;

    // Lê o grafo de grafo.txt
    lerGrafo("grafo.txt");

    // Aplica o algoritmo de Dijkstra e exporta os resultados
    start = clock();
    exportarCaminhosMaisCurtos("resultado-sequencial.txt");
    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Caminhos mais curtos exportados para o arquivo resultado-sequencial.txt\n");
    printf("Tempo de execução: %f segundos\n", cpu_time_used);

    return 0;
}
