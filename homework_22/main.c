// Fibonacci Heap

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define STREQ(a, b) (strcmp((a), (b)) == 0)
#define SWAP(a, b, tmp) {(tmp) = (a); (a) = (b); (b) = (tmp);}

typedef struct Node {
    int key;
    int value;
    int degree; // number of children
    bool mark;  // mark if the child was ever cut
    struct Node* parent;
    struct Node* child;
    struct Node* left;
    struct Node* right;
} Node;

Node* newNode(int key, int value) {
    Node* node = malloc(sizeof(Node));
    node->key = key;
    node->value = value;
    node->degree = 0;
    node->mark = false;
    node->parent = NULL;
    node->child = NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

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
        freeNode(tmp->child);
        free(tmp); tmp = NULL;
        tmp = this;
    }
}

void printNode(Node* this) {
    if (this)
        printf("(%d)%d[%d]\n", this->key, this->value, this->degree);
    else
        printf("(_)_[_]\n");
}

typedef struct FHeap {
    Node* min;
    int nodes;
} FHeap;

FHeap* newHeap() {
    FHeap* this = malloc(sizeof(FHeap));
    this->min = NULL;
    this->nodes = 0;
    return this;
}

void freeHeap(FHeap* this) {
    if (!this) return;
    freeNode(this->min);
    free(this); this = NULL;
}

void insert(FHeap* this, int key, int value) {
    if (!this) {
        fprintf(stderr, "ERROR: B-heap does not exist\n");
        exit(EXIT_FAILURE);
    }

    Node* node = newNode(key, value);

    if (!this->min) {
        node->left = node;
        node->right = node;
        this->min = node;
    } else {
        // insert into the min node's right hand side
        node->left = this->min;
        node->right = this->min->right;
        this->min->right->left = node;
        this->min->right = node;
        if (node->key < this->min->key) this->min = node;
    }
    this->nodes++;
}

void printTable(Node** table, int n) {
    for (int i = 0; i < n; i++) {
        printf("table[%d] = ", i);
        if (table[i])
            printf("(%d)%d\n", table[i]->key, table[i]->value);
        else
            printf("(_)_\n");
    }
}

void meld(FHeap* this) {
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
        while (treeTable[curr->degree]) {  // if there are nodes with the same degree as curr
            Node* small = curr; // tree with smaller root
            Node* large = treeTable[curr->degree]; // tree with larger root
            if (large->key < small->key) {
                large = curr;
                small = treeTable[curr->degree];
            }
            treeTable[curr->degree] = NULL;
            large->mark = false;

            // combine trees (small as parent, large as child)
            if (!small->child) {
                large->left = large;
                large->right = large;
                large->parent = small;
                small->child = large;
            } else {
                Node* head = small->child;
                Node* tail = small->child->left;
                large->left = tail;
                large->right = head;
                large->parent = small;
                head->left = large;
                tail->right = large;
            }
            small->degree++;
            curr = small;
        }
        treeTable[curr->degree] = curr;
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
            // insert the node into the tail's right hand side
            treeTable[i]->left = tail;
            tail->right = treeTable[i];
            tail = tail->right;
            if (tail->key < this->min->key)
                this->min = tail;
        }
    }
    head->left = tail;
    tail->right = head;
    
    free(treeTable); treeTable = NULL;
}

int deleteMin(FHeap* this) {
    if (!this || !this->min) {
        fprintf(stderr, "ERROR: B-heap does not exist or is empty\n");
        exit(EXIT_FAILURE);
    }
    
    // 1. handle the return value
    int min = this->min->value;
    this->nodes--;

    // 2. delete binomail tree with min root and reinsert its children
    // 2.1 move the ownership of min node from this->min to temp
    Node* temp = this->min;  // min node to be deleted
    this->min = NULL;
    
    // 2.2 make children list without temp
    Node* children = NULL;  // children list without temp
    if (temp->left) {
        this->min = children = temp->child;
    }

    // 2.3 make siblings list (root list) without temp
    Node* siblings = NULL;  // siblings list (root list) without temp
    if (temp->right != temp) {
        temp->right->left = temp->left;
        temp->left->right = temp->right;
        this->min = siblings = temp->right;
    }

    // 2.3 merge the children list and siblings list (root list)
    if (children && siblings) {
        Node* children_tail = children->left;
        Node* siblings_tail = siblings->left;
        children_tail->right = siblings;
        siblings_tail->right = children;
        children->left = siblings_tail;
        siblings->left = children_tail;
    }
    
    // 2.4 do the deletion
    free(temp); temp = NULL;
    if (!this->min) return min;

    // 3. meld the binomial trees with the same degrees
    meld(this);

    // 4. update min pointer
    Node* head = this->min;
    Node* iter = head;
    do {
        if (iter->key < this->min->key)
            this->min = iter;
        iter = iter->right;
        iter->parent = NULL;
    } while (iter != head);

    return min;
}

void extract(FHeap* this) {
    if (!this) {
        fprintf(stderr, "ERROR: B-heap does not exist\n");
        exit(EXIT_FAILURE);
    }

    printf("(%d)%d\n", this->min->key, this->min->value);
    deleteMin(this);
}

Node* find(Node* head, int key, int value) {
    if (!head) return NULL;
    Node* iter = head;
    do {
        if (iter->key == key && iter->value == value) {
            return iter;
        } else if (iter->key <= key) {
            Node* node = find(iter->child, key, value);
            if (node) return node;
        }
        iter = iter->right;
    } while (iter != head);
    return NULL;
}

void cut(FHeap* this, Node* parent, Node* grandParent) {
    if (parent == parent->right)
        parent->child = NULL;
    
    parent->left->right = parent->right;
    parent->right->left = parent->left;
    if (parent == grandParent->child)
        grandParent->child = parent->right;
    grandParent->degree--;

    parent->right = this->min;
    parent->left = this->min->left;
    this->min->left->right = parent;
    this->min->left = parent;

    parent->parent = NULL;
    parent->mark = false;
}

void cascadingCut(FHeap* this, Node* parent) {
    Node* grandParent = parent->parent;
    if (!grandParent) return;
    if (!parent->mark) {
        parent->mark = true;
    } else {
        cut(this, parent, grandParent);
        cascadingCut(this, grandParent);
    }
}

void delete(FHeap* this, int key, int value) {
    if (!this) {
        fprintf(stderr, "ERROR: B-heap does not exist\n");
        exit(EXIT_FAILURE);
    }
    
    // 1. if (k, v) == min, then delete min => O(log n)
    if (key == this->min->key && value == this->min->value) {
        deleteMin(this);
        return;
    }

    // 2. else delete (k, v) from list => O(1)
    Node* node = find(this->min, key, value);
    if (!node) return;

    // 3. relink the siblings
    if (node->parent->child == node) {
        node->parent->child = node->right;
    }
    node->left->right = node->right;
    node->right->left = node->left;

    // 4. relink the children to the root list
    Node* child_head = node->child;
    Node* iter = child_head;
    do {
        iter->parent = NULL;
        iter->mark = false;
        iter = iter->right;
    } while (iter != child_head);

    Node* root_head = this->min;
    Node* root_tail = this->min->left;
    Node* child_tail = child_head->left;

    child_tail->right = root_head;
    root_tail->right = child_head;
    child_head->left = root_tail;
    root_head->left = child_tail;

    // 5. cascading cut
    cascadingCut(this, node->parent);

    free(node); node = NULL;
}

void decrease(FHeap* this, int key, int value, int diff) {
    if (!this) {
        fprintf(stderr, "ERROR: B-heap does not exist\n");
        exit(EXIT_FAILURE);
    }

    // 1. decrease the key

    // 2. if key < parent.key, then move (k, v) (and its children) to root list

    // 3. update min pointer

    // 4. cascading cut

}

void preorder(Node* this) {
    if (!this) return;
    Node* iter = this;
    do {
        printf("(%d, %d)%d ", iter->degree, iter->key, iter->value);
        preorder(iter->child);
        iter = iter->right;
    } while (iter != this);
}

int main(void) {
    // char cmd[9] = {0};
    // int key, val, diff;

    FHeap* heap = newHeap();

    insert(heap, 1, 1);
    insert(heap, 4, 4);
    insert(heap, 3, 3);
    insert(heap, 2, 2);
    preorder(heap->min); printf("\n");

    Node* res = find(heap->min, 4, 4);
    printNode(res);

    printf("melt: ");
    meld(heap);
    preorder(heap->min); printf("\n");

    printf("delete ");
    extract(heap);
    preorder(heap->min); printf("\n");

    printf("delete ");
    delete(heap, 3, 3);
    preorder(heap->min); printf("\n");
    
    // do {
    //     scanf("%s", cmd);
    //     if (STREQ(cmd, "insert")) {
    //         scanf("%d %d", key, val);
    //         insert(heap, key, val);
    //     } else if (STREQ(cmd, "extract")) {
    //         extract(heap);
    //     } else if (STREQ(cmd, "delete")) {
    //         scanf("%d %d", key, val);
    //         delete(heap, key, val);
    //     } else if (STREQ(cmd, "decrease")) {
    //         scanf("%d %d %d", key, val, diff);
    //         decrease(heap, key, val, diff);
    //     }
    // } while (!STREQ(cmd, "quit"));

    freeHeap(heap);
    return 0;
}
