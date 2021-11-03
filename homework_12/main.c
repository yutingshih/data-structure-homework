// Min Heap

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define STREQ(a, b) (strcmp((a), (b)) == 0)

typedef struct Node {
    unsigned int key;
} Node;

typedef struct Heap {
    Node* data;
    unsigned int size;
    unsigned int capacity;
} Heap;

Heap* create(const int capacity) {
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    heap->data = (Node*)malloc(sizeof(Node) * capacity);
    heap->capacity = capacity;
    heap->size = 0;
    return heap;
}

void delete(Heap* heap) {
    if (heap) {
        if (heap->data) {
            free(heap->data);
            heap->data = NULL;
        }
        free(heap);
        heap = NULL;
    }
}

bool full(Heap* heap) { return heap->size >= heap->capacity; }
bool empty(Heap* heap) { return heap->size == 0; }

void insert(Heap* heap, const Node item) {
    if (full(heap)) {
        // if the heap is full, then double the capacity of heap
        Node* old_data = heap->data;
        heap->capacity *= 2;
        heap->data = (Node*)malloc(sizeof(Node) * heap->capacity);
        for (unsigned int i = 0; i < heap->size; i++) {
            heap->data[i] = old_data[i];
        }
        free(old_data);
        old_data = NULL;
    }
    
    unsigned int i = (heap->size)++;
    while (i && item.key < heap->data[i/2].key) {
        heap->data[i] = heap->data[i/2];
        i /= 2;
    }
    heap->data[i] = item;
}

Node pop(Heap* heap) {
    if (empty(heap)) {
        fprintf(stderr, "attempt to pop from empty heap\n");
        exit(EXIT_FAILURE);
    }

    Node item = heap->data[0];
    Node temp = heap->data[--(heap->size)];

    unsigned int parent = 0, child = 1, max = heap->size - 1;
    while (child <= max) {
        if (child < max && heap->data[child].key > heap->data[child+1].key)
            child++;

        if (temp.key <= heap->data[child].key) break;

        heap->data[parent] = heap->data[child];
        parent = child;
        child *= 2;
    }
    heap->data[parent] = temp;
    return item;
}

void _heapify(Heap* heap, unsigned int i) {
    unsigned int min = i;
    unsigned int left = 2 * i + 1;
    unsigned int right = 2 * i + 2;
    
    // find the index of minimum element over the parent i and its children
    if (left < heap->size && heap->data[left].key < heap->data[min].key)
        min = left;
    if (right < heap->size && heap->data[right].key < heap->data[min].key)
        min = right;

    if (min != i) {
        int temp = heap->data[i].key;
        heap->data[i].key = heap->data[min].key;
        heap->data[min].key = temp;

        // recursively heapify the affected subtree
        _heapify(heap, min);
    }
}
void heapify(Heap* heap) { _heapify(heap, 0); }

void relabel(Heap* heap, const int old_key, const int new_key) {
    if (empty(heap)) return;

    for (unsigned int i = 0; i < heap->size; i++) {
        if (heap->data[i].key == old_key) {
            heap->data[i].key = new_key;
            break;
        }
    }

    heapify(heap);
}

void levelOrder(Heap* heap) {
    printf("heap size: %d/%d %s%s\n",
            heap->size, heap->capacity,
            full(heap) ? "(full)" : "",
            empty(heap) ? "(empty)" : "");

    if (empty(heap)) return;

    unsigned int n = 0, k = 1;
    for (unsigned int i = 0; i < heap->size; i++) {
        printf("%d ", heap->data[i].key);
        if (i == n) {
            printf("\n");
            k *= 2;
            n += k;
        }
    }
    printf("\n");
}

void prompt() {
    printf("\n");
    printf("new N     - create a heap with capacity N\n");
    printf("push K    - push a node with priority K into the heap\n");
    printf("pop       - pop the minimum value from the heap\n");
    printf("move P Q  - change the node with priority P to be Q\n");
    printf("quit      - quit this program\n");
    printf("help      - show this help message\n");
    printf("\n");
}

int main(void) {
    char command[5] = "";
    Heap* heap = NULL;

    while (!STREQ(command, "quit")) {
        prompt();
        printf(">>> ");
        scanf("%s", command);

        int a, b;
        if (STREQ(command, "new") && !heap) {
            scanf("%d", &a);
            create(a);
            printf("heap with capacity %d created\n", a);

        } else if (STREQ(command, "push") && heap) {
            scanf("%d", &a);
            insert(heap, (Node){a});
            printf("%d inserted\n", a);

        } else if (STREQ(command, "pop") && heap) {
            Node n = pop(heap);
            printf("%d popped\n", n.key);

        } else if (STREQ(command, "move") && heap) {
            scanf("%d %d", &a, &b);
            relabel(heap, a, b);
            printf("node with priority %d is changed to be %d\n", a, b);

        } else if (STREQ(command, "help")) {
            prompt();
        
        } else if (STREQ(command, "quit")) {
            printf("bye bye\n");
            break;

        } else {
            printf("invalid command\n");
        }
    }

    delete(heap);
    return 0;
}

