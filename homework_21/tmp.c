#include <stdio.h>
#include <stdlib.h>

// general tree node for leftist tree and binomial heap
typedef struct Node {
    int data;
    struct Node* left;  // leftChild or child
    struct Node* right; // rightChild or sibling
    int key;    // shortest or degree
} Node;

typedef struct BHeap {
    Node* min;  // head node of root list that points to the min node
    int nodes;
} BHeap;

int bhDeleteMin(BHeap* this) {
    if (!this || !this->min) {
        fprintf(stderr, "ERROR: B-heap does not exist or is empty\n");
        exit(EXIT_FAILURE);
    }
    
    // 1. handle the return value
    int min = this->min->data;
    this->nodes--;

    // 2. delete binomail tree with min root and reinsert its children
    Node* temp = this->min;  // min node to be deleted
    this->min = NULL;

    if (temp->left && temp->right != temp) {
        this->min = temp->left;
        // if there are multiple trees in the root list, then combine
        // the children list of min node and the root list without min node
        Node* tail = temp;   // tail of the root list
        while (tail->right != temp)
            tail = tail->right;
        tail->right = temp->left;

        tail = temp->left;   // tail of the children list
        while (tail->right != temp->left)
            tail = tail->right;
        tail->right = temp->right;
    }
    else if (temp->right != temp) {
        this->min = temp->right;

        Node* tail = temp;  // tail of the root list
        while (tail->right != temp)
            tail = tail->right;
        tail->right = temp->right;
    }
    else if (temp->left) {
        this->min = temp->left;
    }
    
    free(temp); temp = NULL;
    if (!this->min) return min;

    // 3. meld the binomial trees with the same degrees
    bhMeld(this);

    // 4. update min pointer
    Node* head = this->min;
    printf("min: %d(%d)\n", this->min->data, this->min->key);
    Node* iter = head;
    do {
        if (iter->data < this->min->data)
            this->min = iter;
        iter = iter->right;
    } while (iter != head);

    return min;
}