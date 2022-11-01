// Compare the performance of Leftist LeftistTree to Min Heap

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    free(tree);
}

// Binomial Heap
typedef struct HeapNode {
    int data;
    struct HeapNode* child;
    struct HeapNode* sibling;
    int degree;
} HeapNode;

typedef struct BHeap {
    HeapNode* min;
    int maxDegree;
} BHeap;

BHeap* bhMerge(BHeap* this, BHeap* that) {
    if (!that->min) return this;
    if (!this->min) {
        *this = *that;
        that->min = NULL;
        return this;
    }

    // merge root lists
    HeapNode* tmp = this->min->sibling;
    this->min->sibling = that->min->sibling;
    that->min->sibling = tmp;

    // update min
    if (that->min->data < this->min->data)
        *this = *that;
    
    that->min = NULL;
    return this;
}

void bhInsert(BHeap* this, int data) {
    HeapNode* node = malloc(sizeof(HeapNode));

    if (!this->min) {
        *node = (HeapNode){data, NULL, NULL, 1};
        this->min = node;
    } else {
        *node = (HeapNode){data, NULL, this->min->sibling, 1};
        this->min->sibling = node;
        if (data < this->min->data) this->min = node; // update min
    }
}

void bhCombine(BHeap* this) {
    // pairwise combine the trees with the same degrees
    HeapNode** treeTable = calloc(this->maxDegree + 1, sizeof(HeapNode*));

    HeapNode* iter = this->min; // the node used to iterate over the root list
    do {
        if (!treeTable[iter->degree]) {
            treeTable[iter->degree] = iter;
        } else {
            HeapNode* temp = treeTable[iter->degree];
            if (iter->data < temp->data) {
                iter->sibling = temp->sibling;
                temp->sibling = iter->child->sibling;
                iter->child->sibling = temp;
            } else {
                temp->sibling = iter->sibling;
                iter->sibling = temp->child->sibling;
                temp->child->sibling = iter;
                iter = temp;
            }
        }
        iter = iter->sibling;
    } while (iter != this->min);
    
    free(treeTable); treeTable = NULL;
}

void bhDeleteMin(BHeap* this, int* min) {
    HeapNode* popped = this->min;  // min node to be deleted
    *min = popped->data;

    HeapNode* iter = this->min = this->min->sibling;   // iterator to find the end of list
    while (iter->sibling != popped) {
        iter = iter->sibling;
    }
    iter->sibling = popped->sibling;
    free(popped); popped = NULL;

    // merge subtrees into the root list
    HeapNode* children = this->min->child;
    HeapNode* tmp = this->min->sibling;
    this->min->sibling = children->sibling;
    children->sibling = tmp;

    bhCombine(this);

    HeapNode* head = this->min;
    iter = head;
    do {
        if (iter->data < this->min->data) {
            this->min = iter;
        }
        iter = iter->sibling;
    } while (iter != head);
}

void nodeFree(HeapNode* this) {
    if (!this) return;
    if (this->child) nodeFree(this->child);
    HeapNode* tmp = this;
    while (tmp && tmp->sibling) {
        this = this->sibling;
        free(tmp); tmp = NULL;
        tmp = this;
    }
    free(this); this = NULL;
}

void bhFree(BHeap* this) {
    if (!this) return;
    if (this->min) nodeFree(this->min);
    free(this); this = NULL;
}

void inorder(LeftistTree tree) {
    if (!tree) return;
    inorder(tree->left);
    printf("%d ", tree->key);
    inorder(tree->right);
}

void preorder(LeftistTree tree) {
    if (!tree) return;
    printf("%d ", tree->key);
    preorder(tree->left);
    preorder(tree->right);
}

void postorder(LeftistTree tree) {
    if (!tree) return;
    postorder(tree->left);
    postorder(tree->right);
    printf("%d ", tree->key);
}

void in(HeapNode* this) {
    if (!this) return;
    if (this->child) in(this->child);
    printf("%d %d\n", this->data, this->degree);
    if (this->sibling) in(this->sibling);
}

int main_(void) {
    const int numElem[] = {100, 500, 1000, 2000, 3000, 4000, 5000};
    const int numOp = 5000;

    for (int n = 0; n < sizeof(numElem) / sizeof(int); n++) {
        LeftistTree tree = NULL;
        // BHeap* heap = NULL;
        int data[numElem[n]];
        for (int i = 0; i < numElem[n]; i++) {
            data[i] = rand();
            leftistInsert(&tree, data[i]);
            // bhInsert(heap, data[i]);
        }

        int tmp = 0;
        for (int i = 0; i < numOp; i++) {
            switch (rand() % 2) {
                case 0: leftistInsert(&tree, rand()); break;
                case 1: leftistDeleteMin(&tree, &tmp); break;
                default: break;
            }
        }

        leftistFree(tree); tree = NULL;
    }
    return 0;
}

int main(void) {
    BHeap* heap = NULL;
    *heap = (BHeap) {
        .min = NULL,
        .maxDegree = 0,
    };
    // bhInsert(heap, 10);
    // bhInsert(heap, 20);
    // bhInsert(heap, 30);
    // bhInsert(heap, 40);
    // in(heap->min);
    return 0;
}
