#include <stdio.h>
#include <time.h>
#include <math.h>

#define INT_MAX 100000
#define TRUE    1
#define FALSE   0
#define V 1000
#define E 4000

// Boolean type
typedef int bool;

// Represents an edge or path between Vertices
typedef struct {
    int u;
    int v;
} Edge;

// Represents a Vertex
typedef struct {
    int title;
    bool visited;
} Vertex;

// Prints the array
void printArray(int *array) {
    for (int i = 0; i < V; i++) {
        // printf("Path to Vertex %d is %d\n", i, array[i]);
    }
}

// Sequential Implementation of Dijkstra's Algorithm
void DijkstraSequential(Vertex *vertices, Edge *edges, int *weights, Vertex *root) {
    double start, end;

    root->visited = TRUE;
    int len[V];
    len[root->title] = 0;

    // Initialize distances
    for (int i = 0; i < V; i++) {
        if (vertices[i].title != root->title) {
            len[vertices[i].title] = findEdge(*root, vertices[i], edges, weights);
        } else {
            vertices[i].visited = TRUE;
        }
    }

    start = (double)clock() / CLOCKS_PER_SEC; // Start time

    for (int j = 0; j < V; j++) {
        Vertex u;
        int h = minPath(vertices, len);
        u = vertices[h];

        // Sequentially update distances
        for (int i = 0; i < V; i++) {
            if (vertices[i].visited == FALSE) {
                int c = findEdge(u, vertices[i], edges, weights);
                len[vertices[i].title] = minimum(len[vertices[i].title], len[u.title] + c);
            }
        }
    }

    end = (double)clock() / CLOCKS_PER_SEC; // End time
    printArray(len);
    printf("Running time: %f ms\n", (end - start) * 1000);
}

// Finds the edge that connects Vertex u with Vertex v
int findEdge(Vertex u, Vertex v, Edge *edges, int *weights) {
    for (int i = 0; i < E; i++) {
        if (edges[i].u == u.title && edges[i].v == v.title) {
            return weights[i];
        }
    }
    return INT_MAX;
}

// Returns the minimum between two integers
int minimum(int A, int B) {
    return (A > B) ? B : A;
}

// Visits the vertices and looks for the lowest weight from the vertex
int minWeight(int *len, Vertex *vertices) {
    int minimum = INT_MAX;
    for (int i = 0; i < V; i++) {
        if (vertices[i].visited == FALSE && len[i] < minimum) {
            minimum = len[i];
        }
    }
    return minimum;
}

// Localizes the vertex with the lowest weight path
int minPath(Vertex *vertices, int *len) {
    int min = minWeight(len, vertices);
    for (int i = 0; i < V; i++) {
        if (vertices[i].visited == FALSE && len[vertices[i].title] == min) {
            vertices[i].visited = TRUE;
            return i;
        }
    }
    return -1; // In case no valid vertex is found
}

int main(void) {
    Vertex vertices[V];
    Edge edges[E];
    int weights[E];

    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    // Inicializa os vértices
    for (int i = 0; i < V; i++) {
        Vertex a = {.title = i, .visited = FALSE};
        vertices[i] = a;
    }

    // Gera arestas e pesos aleatórios
    for (int i = 0; i < E; i++) {
        edges[i].u = rand() % V; // Vértice inicial da aresta
        edges[i].v = rand() % V; // Vértice final da aresta
        weights[i] = rand() % 11; // Peso da aresta (0 a 10)
    }

    // Seleciona o vértice raiz
    Vertex root = {0, FALSE};

    printf("Resultados para grafo aleatório com %d vértices:\n", V);
    DijkstraSequential(vertices, edges, weights, &root);

    return 0;
}
