// Depth-First Search (DFS)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Graph {
    int numNodes;
    bool** adjMatrix;
    bool* visited;
} Graph;

Graph* createGraph(const int numNodes) {
    Graph* self = (Graph*)malloc(sizeof(Graph));
    *self = (Graph){
        .numNodes = numNodes,
        .adjMatrix = (bool**)malloc(sizeof(bool*) * numNodes),
        .visited = (bool*)calloc(numNodes, sizeof(bool)),
    };
    for (int i = 0; i < numNodes; i++) {
        self->adjMatrix[i] = (bool*)calloc(numNodes, sizeof(bool));
    }
    return self;
}

void deleteGraph(Graph* self) {
    if (!self) return;

    if (self->visited) {
        free(self->visited);
        self->visited = NULL;
    }

    if (self->adjMatrix) {
        for (int i = 0; i < self->numNodes; i++) {
            if (self->adjMatrix[i]) {
                free(self->adjMatrix[i]);
                self->adjMatrix[i] = NULL;
            }
        }
        free(self->adjMatrix);
        self->adjMatrix = NULL;
    }

    free(self);
    self = NULL;
}

void dfs(Graph* self, const int entry) {
    if (!self || entry < 0 || entry >= self->numNodes) {
        fprintf(stderr, "dfs entry point %d does not exist\n", entry);
        exit(EXIT_FAILURE);
    }

    printf("%d ", entry);
    self->visited[entry] = true;
    for (int i = 0; i < self->numNodes; i++) {
        if (self->adjMatrix[entry][i] && !self->visited[i]) {
            dfs(self, i);
        }
    }
}

void readGraph(Graph* self, FILE* stream) {
    if (!self) {
        fprintf(stderr, "graph does not exist\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < self->numNodes; i++) {
        for ( int j = 0; j < self->numNodes; j++) {
            int temp = 0;
            fscanf(stream, "%d ", &temp);
            self->adjMatrix[i][j] = (bool)temp;
        }
    }
}

void writeGraph(Graph* self, FILE* stream) {
    if (!self) {
        fprintf(stderr, "graph does not exist\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < self->numNodes; i++) {
        for (int j = 0; j < self->numNodes; j++) {
            fprintf(stream, "%d ", self->adjMatrix[i][j]);
        }
        fprintf(stream, "\n");
    }
    fprintf(stream, "\n");
}

int main(void) {
    int numCases = 0;
    scanf("%d", &numCases);
    for (int n = 0; n < numCases; n++) {
        int numNodes = 0, entry = 0;
        scanf("%d %d", &numNodes, &entry);
        Graph* graph = createGraph(numNodes);
        readGraph(graph, stdin);
        dfs(graph, entry);
        printf("\n");
        deleteGraph(graph);
    }
    return 0;
}

