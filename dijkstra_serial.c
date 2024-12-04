#include <stdio.h>
#include <time.h>
#include <math.h>
#include <omp.h>

#define INT_MAX 100000  // Define um valor grande para representar "infinito"
#define TRUE 1
#define FALSE 0
#define V 1000  // Número de vértices
#define E 4000  // Número de arestas

typedef int bool;

typedef struct {
    int u;  // Vértice de origem da aresta
    int v;  // Vértice de destino da aresta
} Edge;

typedef struct {
    int title;     // Identificador do vértice
    bool visited;  // Indica se o vértice já foi visitado
} Vertex;

// Declaração de funções auxiliares
int minPath(Vertex *vertices, int *len);
int minWeight(int *len, Vertex *vertices);
int minimum(int A, int B);
int findEdge(Vertex u, Vertex v, Edge *edges, int *weights);
void DijkstraOMP(Vertex *vertices, Edge *edges, int *weights, Vertex *root);
void printArray(int *array);

void printArray(int *array) {
    int i;
    for (i = 0; i < V; i++) {
        // Comentado para evitar sobrecarregar a saída
        // printf("Caminho mais curto do vertice %d: %d\n", i, array[i]);
    }
}

void DijkstraOMP(Vertex *vertices, Edge *edges, int *weights, Vertex *root) {
    double start, end;

    root->visited = TRUE;  // Marca o vértice inicial como visitado

    int len[V];            // Vetor para armazenar os caminhos mínimos
    len[(int)root->title] = 0;  // O custo para o vértice inicial é 0

    int i, j;

    start = omp_get_wtime();  // Marca o início da execução paralelizada

    // Inicializa os custos para cada vértice
    for (i = 0; i < V; i++) {
        if (vertices[i].title != root->title) {
            // Encontra o peso da aresta entre o vértice inicial e os demais
            len[(int)vertices[i].title] = findEdge(*root, vertices[i], edges, weights);
        } else {
            vertices[i].visited = TRUE;  // Marca o vértice inicial como visitado
        }
    }

    // Itera sobre todos os vértices para calcular os caminhos mínimos
    for (j = 0; j < V; j++) {
        Vertex u;
        int h = minPath(vertices, len);  // Encontra o próximo vértice com menor peso
        u = vertices[h];                // Atualiza o vértice corrente

        // Paraleliza a atualização dos custos dos vizinhos do vértice atual
        #pragma omp parallel for schedule(runtime) private(i)
        for (i = 0; i < V; i++) {
            if (vertices[i].visited == FALSE) {  // Apenas para vértices não visitados
                int c = findEdge(u, vertices[i], edges, weights);  // Peso da aresta
                len[vertices[i].title] = minimum(len[vertices[i].title], len[u.title] + c);
            }
        }
    }

    end = omp_get_wtime();  // Marca o fim da execução paralelizada

    // Imprime o tempo de execução (em ms)
    printf("Tempo de execucao: %f ms\n", (end - start) * 1000);
}

int findEdge(Vertex u, Vertex v, Edge *edges, int *weights) {
    int i;
    for (i = 0; i < E; i++) {
        if (edges[i].u == u.title && edges[i].v == v.title) {
            return weights[i];  // Retorna o peso da aresta, se existir
        }
    }
    return INT_MAX;  // Retorna "infinito" se não houver aresta
}

int minimum(int A, int B) {
    // Retorna o menor entre dois números
    if (A > B) {
        return B;
    } else {
        return A;
    }
}

int minWeight(int *len, Vertex *vertices) {
    // Encontra o menor custo entre os vértices não visitados
    int i;
    int minimum = INT_MAX;
    for (i = 0; i < V; i++) {
        if (vertices[i].visited == TRUE) {
            continue;  // Ignora vértices já visitados
        } else if (vertices[i].visited == FALSE && len[i] < minimum) {
            minimum = len[i];
        }
    }
    return minimum;
}

int minPath(Vertex *vertices, int *len) {
    // Encontra o índice do vértice não visitado com o menor custo
    int i;
    int min = minWeight(len, vertices);

    for (i = 0; i < V; i++) {
        if (vertices[i].visited == FALSE && len[vertices[i].title] == min) {
            vertices[i].visited = TRUE;  // Marca como visitado
            return i;  // Retorna o índice do vértice
        }
    }
    return -1;  // Caso todos os vértices tenham sido visitados
}

int main(void) {
    Vertex vertices[V];  // Cria os vértices
    Edge edges[E];       // Cria as arestas
    int weights[E];      // Cria os pesos das arestas

    srand(0);  // Inicializa o gerador de números aleatórios

    // Inicializa os vértices
    for (int i = 0; i < V; i++) {
        Vertex a = {.title = i, .visited = FALSE};
        vertices[i] = a;
    }

    // Inicializa as arestas e seus pesos
    for (int i = 0; i < E; i++) {
        edges[i].u = rand() % V;
        edges[i].v = rand() % V;
        weights[i] = rand() % 11;  // Pesos entre 0 e 10
    }

    Vertex root = {0, FALSE};  // Define o vértice inicial (root)

    printf("Resultados para grafo aleatório com %d vértices:\n", V);
    DijkstraOMP(vertices, edges, weights, &root);  // Executa o algoritmo de Dijkstra paralelizado

    return 0;
}
