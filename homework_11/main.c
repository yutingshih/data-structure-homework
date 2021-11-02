// Threaded Binary Tree Insertion

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define STREQ(a, b) (strcmp(a, b) == 0)

typedef struct node {
    int key;
    bool leftThread;
    bool rightThread;
    struct node* leftChild;
    struct node* rightChild;
} node;

node* createNode(int key) {
    node* n = (node*)malloc(sizeof(node));
    if (!n) {
        fprintf(stderr, "Insufficient memory\n");
        exit(EXIT_FAILURE);
    }
    *n = (node){
        .key = key,
        .leftChild = n,
        .leftThread = true,
        .rightChild = n,
        .rightThread = false,
    };
    return n;
}

void deleteTree(node* root) {
    if (!root) return;

    if (!root->leftThread)
        deleteTree(root->leftChild);
    
    if (!root->rightThread && root->key != -1)
        deleteTree(root->rightChild);

    free(root);
    root = NULL;
}

node* next(const node* curr) {
    node* res = curr->rightChild;
    // if curr has right subtree, find the leftmost node of the subtree
    if (!curr->rightThread)
        while (!res->leftThread)
            res = res->leftChild;
    return res;
}

node* prev(const node* curr) {
    node* res = curr->leftChild;
    // if curr has left subtree, find the rightmost node of the subtree
    if (!curr->leftThread)
        while (!res->rightThread)
            res = res->rightChild;
    return res;
}

node* inorderSearch(node* root, const int id) {
    node* curr = root;
    for (; curr->key != id; curr = next(curr)) {
        if (curr == next(curr)) {
            return NULL;
        }
    }
    return curr;
}

void inorderTraverse(node* root) {
    node* curr = next(root);
    do {
        printf("%d ", curr->key);
        curr = next(curr);
    } while (curr != root);
    printf("\n");
}

void insertRight(node* parent, node* child) {
    child->leftChild = parent;
    child->leftThread = true;
    child->rightChild = parent->rightChild;
    child->rightThread = parent->rightThread;
    
    parent->rightChild = child;
    parent->rightThread = false;

    if (!child->rightThread) {
        node* tmp = next(child);
        tmp->leftChild = child;
    }
}

void insertLeft(node* parent, node* child) {
    child->leftChild = parent->leftChild;
    child->leftThread = parent->leftThread;
    child->rightChild = parent;
    child->rightThread = true;

    parent->leftChild = child;
    parent->leftThread = false;

    if (!child->leftThread) {
        node* tmp = prev(child);
        tmp->rightChild = child;
    }
}

int main(void) {
    int num, id;
    scanf("%d %d", &num, &id);
    node* root = createNode(-1);
    insertLeft(root, createNode(id));

    int parentId, childId;
    char operation[6] = "";
    for (int i = 0; i < num; i++) {
        scanf("%d %s %d", &parentId, operation, &childId);
        node* parent = inorderSearch(root, parentId);
        node* child = createNode(childId);

        if (STREQ(operation, "left")) {
            insertLeft(parent, child);
        } else if (STREQ(operation, "right")) {
            insertRight(parent, child);
        }
    }

    inorderTraverse(root);
    deleteTree(root);

    return 0;
}
