#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <omp.h>

#define INF INT_MAX
#define MAX_VERTICES 1000

int graph[MAX_VERTICES][MAX_VERTICES];
int VERTICES;

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

void dijkstra(int start, int *dist) {
    int visited[MAX_VERTICES] = {0};

    // Inicializa distâncias com infinito e a distância do vértice inicial como 0
    for (int i = 0; i < VERTICES; i++) {
        dist[i] = INF;
    }
    dist[start] = 0;

    // Algoritmo de Dijkstra otimizado
    for (int count = 0; count < VERTICES - 1; count++) {
        int u = -1;
        int min_dist = INF;

        // Encontrar o vértice com a menor distância (redução paralela)
        #pragma omp parallel for reduction(min:min_dist)
        for (int i = 0; i < VERTICES; i++) {
            if (!visited[i] && dist[i] < min_dist) {
                #pragma omp critical
                {
                    if (dist[i] < min_dist) {
                        min_dist = dist[i];
                        u = i;
                    }
                }
            }
        }

        if (u == -1) break;

        visited[u] = 1;

        // Atualizar distâncias dos vizinhos de u
        #pragma omp parallel for
        for (int v = 0; v < VERTICES; v++) {
            if (!visited[v] && graph[u][v] != INF && dist[u] != INF) {
                int new_dist = dist[u] + graph[u][v];
                #pragma omp critical
                {
                    if (new_dist < dist[v]) {
                        dist[v] = new_dist;
                    }
                }
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

    // Paraleliza os cálculos de Dijkstra para diferentes vértices de partida
    #pragma omp parallel
    {
        int *dist = (int *)malloc(VERTICES * sizeof(int)); // Cópia local de dist para cada thread
        if (dist == NULL) {
            printf("Erro ao alocar memória para o vetor dist\n");
            exit(1);
        }

        #pragma omp for
        for (int i = 0; i < VERTICES; i++) {
            dijkstra(i, dist);

            // Escreve os resultados em regiões críticas para evitar conflitos
            #pragma omp critical
            {
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
        }

        free(dist); // Libera a memória local
    }

    fclose(arquivo);
}

int main() {
    double start, end;

    // Lê o grafo de grafo.txt
    lerGrafo("grafo.txt");

    start = omp_get_wtime();
    exportarCaminhosMaisCurtos("resultado-paralelo.txt");
    end = omp_get_wtime();

    printf("Caminhos mais curtos exportados para o arquivo resultado-paralelo.txt\n");
    printf("Tempo de execução: %f segundos\n", end - start);

    return 0;
}
