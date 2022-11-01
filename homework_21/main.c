// Leftist LeftistTree & Binomial Heap

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define SWAP(a, b, tmp) {(tmp) = (a); (a) = (b); (b) = (tmp);}

// Leftist Tree
typedef struct LeftistNode {
    int key;
    struct LeftistNode* left;
    struct LeftistNode* right;
    int shortest;
} LeftistNode;

typedef LeftistNode* LeftistTree;

LeftistTree* leftistMerge(LeftistTree* a, LeftistTree* b) {
    // handle empty tree
    if (!*b) return a;
    if (!*a) {
        *a = *b;
        *b = NULL;
        return a;
    }

    // make tree a be the tree with smaller root key
    LeftistTree tmp = NULL;
    if ((*a)->key > (*b)->key) SWAP(*a, *b, tmp);

    // leftistMerge b into the right subtree of a
    if ((*a)->right) leftistMerge(&(*a)->right, b);
    else (*a)->right = *b;
    *b = NULL;

    // check definition of leftist (left is not shorter than right)
    if (!(*a)->left) {
        (*a)->left = (*a)->right;
        (*a)->right = NULL;
    } else if ((*a)->left->shortest < (*a)->right->shortest) {
        SWAP((*a)->left, (*a)->right, tmp);
    }

    // maintain the shortest path of each node
    (*a)->shortest = (*a)->right ? (*a)->right->shortest + 1 : 1;
    return a;
}

void leftistInsert(LeftistTree* tree, int key) {
    LeftistTree node = malloc(sizeof(LeftistNode));
    *node = (LeftistNode){key, NULL, NULL, 1};
    leftistMerge(tree, &node);
}

void leftistDeleteMin(LeftistTree* tree, int* min) {
    if (!*tree) {
        min = NULL;
        return;
    }
    LeftistTree tmp = *tree;
    *min = tmp->key;
    *tree = *leftistMerge(&tmp->left, &tmp->right);
    free(tmp);
}

void leftistFree(LeftistTree tree) {
    if (!tree) return;
    if (tree->left) leftistFree(tree->left);
    if (tree->right) leftistFree(tree->right);
    free(tree); tree = NULL;
}

// general tree node for leftist tree and binomial heap
typedef struct Node {
    int data;
    struct Node* left;  // leftChild or child
    struct Node* right; // rightChild or sibling
    int key;    // shortest or degree
} Node;

void freeNode(Node* this) {
    if (!this) return;
    
    // break the ring
    Node* tmp = this;
    this = this->right;
    tmp->right = NULL;
    tmp = this;

    // recursively free the nodes
    while (tmp) {
        this = this->right;
        freeNode(tmp->left);
        free(tmp); tmp = NULL;
        tmp = this;
    }
}

typedef struct BHeap {
    Node* min;  // head node of root list that points to the min node
    int nodes;
} BHeap;

BHeap* bhNew() {
    BHeap* this = malloc(sizeof(BHeap));
    this->min = NULL;
    this->nodes = 0;
    return this;
}

void bhFree(BHeap* this) {
    if (!this) return;
    freeNode(this->min);
    free(this); this = NULL;
}

void bhInsert(BHeap* this, int data) {
    if (!this) {
        fprintf(stderr, "ERROR: B-heap does not exist\n");
        exit(EXIT_FAILURE);
    }

    Node* node = malloc(sizeof(Node));
    node->key = 0;
    node->left = NULL;
    node->data = data;
    this->nodes++;

    if (!this->min) {   // insert the first node
        node->right = node;
        this->min = node;
    } else {
        node->right = this->min->right;
        this->min->right = node;
        if (data < this->min->data) this->min = node; // update min
    }
}

void bhMeld(BHeap* this) {
    if (!this) {
        fprintf(stderr, "ERROR: B-heap does not exist\n");
        exit(EXIT_FAILURE);
    }

    // 1. pairwise combine the trees with the same degrees
    // CAUTION: The root list will be broken, and should be relink later.
    int max_degree = ceil(log2(this->nodes));
    Node** treeTable = calloc(max_degree + 1, sizeof(Node*));
    
    Node* curr = this->min; // current node for iterating the root list
    Node* next = this->min->right; // next node for iterating the root list
    do {
        while (treeTable[curr->key]) {  // if there are nodes with the same degree as curr
            Node* small = curr; // tree with smaller root
            Node* large = treeTable[curr->key]; // tree with larger root
            if (large->data < small->data) {
                large = curr;
                small = treeTable[curr->key];
            }
            treeTable[curr->key] = NULL;

            // combine trees (small as parent, large as child)
            if (!small->left) {
                large->right = large;
                small->left = large;
            } else {
                large->right = small->left->right;
                small->left->right = large;
            }
            small->key++;
            curr = small;
        }
        treeTable[curr->key] = curr;

        curr = next;
        next = next->right;
    } while (curr != this->min);

    // 2. relink the root list
    Node* head = NULL;
    Node* tail = NULL;
    for (int i = 0; i <= max_degree; i++) {
        if (treeTable[i] && !head) {
            this->min = head = tail = treeTable[i];
        } else if (treeTable[i]) {
            tail->right = treeTable[i];
            tail = tail->right;
            if (tail->data < this->min->data)
                this->min = tail;
        }
    }
    tail->right = head;
    
    free(treeTable); treeTable = NULL;
}

int bhDeleteMin(BHeap* this) {
    if (!this || !this->min) {
        fprintf(stderr, "ERROR: B-heap does not exist or is empty\n");
        exit(EXIT_FAILURE);
    }
    
    // 1. handle the return value
    int min = this->min->data;
    this->nodes--;

    // 2. delete binomail tree with min root and reinsert its children
    // 2.1 move the ownership of min node from this->min to temp
    Node* temp = this->min;  // min node to be deleted
    this->min = NULL;
    
    // 2.2 make children list without temp
    Node* children = NULL;  // children list without temp
    if (temp->left) {
        this->min = children = temp->left;
    }

    // 2.3 make siblings list (root list) without temp
    Node* siblings = NULL;  // siblings list (root list) without temp
    if (temp->right != temp) {
        siblings = temp;
        while (siblings->right != temp) 
            siblings = siblings->right;
        siblings->right = temp->right;

        this->min = siblings;
    }

    // 2.3 merge the children list and siblings list (root list)
    if (children && siblings) {
        Node* swap = children->right;
        children->right = siblings->right;
        siblings->right = swap;
    }
    
    // 2.4 do the deletion
    free(temp); temp = NULL;
    if (!this->min) return min;

    // 3. meld the binomial trees with the same degrees
    bhMeld(this);

    // 4. update min pointer
    Node* head = this->min;
    Node* iter = head;
    do {
        if (iter->data < this->min->data)
            this->min = iter;
        iter = iter->right;
    } while (iter != head);

    return min;
}

// void preorder(Node* this) {
//     if (!this) return;
//     Node* iter = this;
//     do {
//         printf("%d(%d) ", iter->data, iter->key);
//         preorder(iter->left);
//         iter = iter->right;
//     } while (iter != this);
// }

// void postorder(Node* this) {
//     if (!this) return;
//     Node* iter = this;
//     do {
//         postorder(iter->left);
//         printf("%d(%d) ", iter->data, iter->key);
//         iter = iter->right;
//     } while (iter != this);
// }

int main(void) {
    const int cases = 7;
    const int numElem[] = {100, 500, 1000, 2000, 3000, 4000, 5000};
    const int numOp = 5000;

    double* timeLT = calloc(cases, sizeof(double)); // result of leftist tree
    double* timeBH = calloc(cases, sizeof(double)); // result of binomial heap

    int initData[5000];
    int operations[numOp];  // 0 for insert, 1 for delete
    int randomData[numOp];
    for (int i = 0; i < numOp; i++) {
        initData[i] = rand();
        operations[i] = rand() % 2;
        randomData[i] = rand();
    }

    for (int n = 0; n < sizeof(numElem) / sizeof(int); n++) {
        // initialize leftist tree and binomial heap
        LeftistTree tree = NULL;
        BHeap* heap = bhNew();
        for (int i = 0; i < numElem[n]; i++) {
            leftistInsert(&tree, initData[i]);
            bhInsert(heap, initData[i]);
        }
        bhMeld(heap);

        // measure the performance of leftist tree
        int tmp = 0;
        clock_t start = clock();
        for (int i = 0; i < numOp; i++) {
            switch (operations[i]) {
                case 0: leftistInsert(&tree, randomData[i]); break;
                case 1: leftistDeleteMin(&tree, &tmp); break;
                default: break;
            }
        }
        clock_t stop = clock();
        timeLT[n] = ((double)(stop - start)) / CLOCKS_PER_SEC;

        // measure the performance of binomial heap
        start = clock();
        for (int i = 0; i < numOp; i++) {
            switch (operations[i]) {
                case 0: bhInsert(heap, randomData[i]); break;
                case 1: bhDeleteMin(heap); break;
                default: break;
            }
        }
        stop = clock();
        timeBH[n] = ((double)(stop - start)) / CLOCKS_PER_SEC;

        // reset leftist tree and binomial heap
        leftistFree(tree);
        bhFree(heap);
    }

    printf("+-------+------------------+------------------+\n");
    printf("|   n   |   leftist tree   |   binomial heap  |\n");
    printf("+-------+------------------+------------------|\n");
    for (int n = 0; n < cases; n++) {
        printf("| %5d |  %12lf    |  %12lf    |\n", numElem[n], timeLT[n], timeBH[n]);
    }
    printf("+-------+------------------+------------------+\n");

    free(timeLT); timeLT = NULL;
    free(timeBH); timeBH = NULL;
    return 0;
}