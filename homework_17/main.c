// Breadth-First Search (BFS)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Queue {
    int capacity;
    int head;
    int tail;
    int* data;
} Queue;

Queue* createQueue(const int capacity) {
    Queue* self = (Queue*)malloc(sizeof(Queue));
    *self = (Queue){
        .capacity = capacity,
        .head = -1,
        .tail = -1,
        .data = (int*)malloc(sizeof(int) * capacity),
    };
    return self;
}

void deleteQueue(Queue* self) {
    if (!self) return;
    
    if (self->data) {
        free(self->data);
        self->data = NULL;
    }
    
    free(self);
    self = NULL;
}

bool empty(Queue* self) {
    return self->head == -1;
}

bool full(Queue* self) {
    return self->head == (self->tail + 1) % self->capacity;
}

void enqueue(Queue* self, const int data) {
    if (full(self)) {
        fprintf(stderr, "try to push into a full queue\n");
        exit(EXIT_FAILURE);
    }

    if (self->head == -1) self->head++;
    self->tail = (self->tail + 1) % self->capacity;
    self->data[self->tail] = data;
}

int dequeue(Queue* self) {
    if (empty(self)) {
        fprintf(stderr, "try to pop from an empty queue\n");
        exit(EXIT_FAILURE);
    }

    int data = self->data[self->head];
    if (self->head == self->tail) {
        self->head = self->tail = -1;
    } else {
        self->head = (self->head + 1) % self->capacity;
    }
    return data;
}

typedef struct Graph {
    int numNodes;
    bool** adjMatrix;
    bool* visited;
    Queue* queue;
} Graph;

Graph* createGraph(const int numNodes) {
    Graph* self = (Graph*)malloc(sizeof(Graph));
    *self = (Graph){
        .numNodes = numNodes,
        .adjMatrix = (bool**)malloc(sizeof(bool*) * numNodes),
        .visited = (bool*)calloc(numNodes, sizeof(bool)),
        .queue = createQueue(numNodes),
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

    if (self->queue) {
        free(self->queue);
        self->queue = NULL;
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

void bfs(Graph* self, const int entry) {
    if (!self || entry < 0 || entry >= self->numNodes) {
        fprintf(stderr, "dfs entry point %d does not exist\n", entry);
        exit(EXIT_FAILURE);
    }
    if (!self->queue) {
        fprintf(stderr, "queue not exist\n");
        exit(EXIT_FAILURE);
    }

    enqueue(self->queue, entry);
    self->visited[entry] = true;
    printf("%d ", entry);

    while (!(empty(self->queue))) {
        int curr = dequeue(self->queue);
        for (int next = 0; next < self->numNodes; next++) {
            if (self->adjMatrix[curr][next] && !self->visited[next]) {
                enqueue(self->queue, next);
                self->visited[next] = true;
                printf("%d ", next);
            }
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
        bfs(graph, entry);
        printf("\n");
        deleteGraph(graph);
    }
    return 0;
}

