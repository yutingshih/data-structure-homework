// Minimum Spanning Tree with Kruskal's Algorithm

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define FREE(ptr) \
    if ((ptr)) { \
        free(ptr); \
        (ptr) = NULL; \
    }

typedef struct Edge {
    int u, v, w;
} Edge;

typedef struct MinHeap {
    Edge* data;
    int size;
    int capacity;
} MinHeap;

MinHeap* newHeap(int capacity) {
    if (capacity < 0) {
        fprintf(stderr, "invalid heap capacity %d\n", capacity);
    }

    MinHeap* self = (MinHeap*)malloc(sizeof(MinHeap));
    *self = (MinHeap){
        .data = (Edge*)malloc(sizeof(Edge) * (capacity + 1)),
        .size = 0,
        .capacity = capacity,
    };
    return self;
}

void freeHeap(MinHeap* self) {
    if (!self) return;
    FREE(self->data);
    FREE(self);
}

bool heapFull(MinHeap* self) {
    return self->size == self->capacity;
}

bool heapEmpty(MinHeap* self) {
    return !self->size;
}

void insertHeap(MinHeap* self, Edge item) {
    if (heapFull(self)) {
        fprintf(stderr, "heap is full\n");
        exit(EXIT_FAILURE);
    }
    int i = ++(self->size);
    while (i > 1 && item.w < self->data[i/2].w) {
        self->data[i] = self->data[i/2];
        i /= 2;
    }
    self->data[i] = item;
}

Edge popHeap(MinHeap* self) {
    if (heapEmpty(self)) {
        fprintf(stderr, "heap is empty\n");
        exit(EXIT_FAILURE);
    }
    
    Edge item = self->data[1];

    Edge temp = self->data[(self->size)--];
    int parent = 1, child = 2;
    while (child <= self->size) {
        if (child < self->size && self->data[child].w > self->data[child+1].w)
            child++;

        if (temp.w <= self->data[child].w) break;

        self->data[parent] = self->data[child];
        parent = child;
        child *= 2;
    }
    self->data[parent] = temp;

    return item;
}

typedef struct DisjointSet {
    int* data;
    int size;
} DisjointSet;

DisjointSet* newSet(int size) {
    DisjointSet* self = (DisjointSet*)malloc(sizeof(DisjointSet));
    *self = (DisjointSet){
        .data = (int*)malloc(sizeof(int) * size),
        .size = size,
    };
    for (int i = 0; i < size; i++) {
        self->data[i] = -1;
    }
    return self;
}

void freeSet(DisjointSet* self) {
    if (!self) return;
    FREE(self->data);
    FREE(self);
}

int setFind(DisjointSet* self, int v) {
    int root;
    int* parent = self->data;
    for (root = v; parent[root] >= 0; root = parent[root]);

    int trail, lead;
    for (trail = v; trail != root; trail = lead) {
        lead = parent[trail];
        parent[trail] = root;
    }
    return root;
}

void setUnion(DisjointSet* self, int a, int b) {
    int* parent = self->data;
    a = setFind(self, a);
    b = setFind(self, b);
    if (parent[a] < parent[b]) {
        parent[a] += parent[b];
        parent[b] = a;
    } else if (parent[b] < parent[a]) {
        parent[b] += parent[a];
        parent[a] = b;
    }
}

bool setSame(DisjointSet* self, int a, int b) {
    return setFind(self, a) == setFind(self, b);
}

unsigned int kruskal(DisjointSet* vertices, MinHeap* edges) {
    unsigned int totalCost = 0;
    int numE = 0, numV = vertices->size;
    while (numE < numV - 1 && !heapEmpty(edges)) {
        Edge minEdge = popHeap(edges);
        if (!setSame(vertices, minEdge.u, minEdge.v)) {
            setUnion(vertices, minEdge.u, minEdge.v);
            totalCost += minEdge.w;
            numE++;
        }
    }
    if (numE < numV - 1) {
        fprintf(stderr, "there is no spanning tree\n");
    }
    return totalCost;
}

int main(void) {
    int numV, numE;
    scanf("%d %d", &numV, &numE);
    DisjointSet* vertices = newSet(numV);
    MinHeap* edges = newHeap(numE);
    
    for (int i = 0; i < numE; i++) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        insertHeap(edges, (Edge){u, v, w});
    }

/*
    for (int i = 0; i < numE; i++) {
        Edge temp = popHeap(edges);
        printf("%d %d %d\n", temp.u, temp.v, temp.w);
    }
*/

    unsigned int totalCost = kruskal(vertices, edges);
    printf("%u\n", totalCost);

    freeSet(vertices);
    freeHeap(edges);
    return 0;
}

