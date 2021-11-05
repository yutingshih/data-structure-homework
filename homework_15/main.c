// Build Binary Tree with Preorder, Postorder and Inorder Traversals

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STREQ(a, b) (strcmp((a), (b)) == 0)

typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
} Node;

Node* newNode(int value, Node* left, Node* right) {
    Node* self = (Node*)malloc(sizeof(Node));
    *self = (Node){value, left, right};
    return self;
}

void freeNode(Node* self) {
    if (!self) return;
    if (self->left) freeNode(self->left);
    if (self->right) freeNode(self->right);
    free(self);
    self = NULL;
}

void preorder(Node* self) {
    if (!self) return;
    printf("%d ", self->value);
    preorder(self->left);
    preorder(self->right);
}

void postorder(Node* self) {
    if (!self) return;
    postorder(self->left);
    postorder(self->right);
    printf("%d ", self->value);
}

Node* buildTreePreIn(int* preorder, int* inorder, const int size) {
    if (!preorder || !inorder || !size) return NULL;
    
    int rootValue = preorder[0];
    int rootIndex = 0;  // index of root in the inorder sequence
    while (rootIndex < size && inorder[rootIndex] != rootValue)
        rootIndex++;

    Node* tree = newNode(
        rootValue,
        buildTreePreIn(preorder+1, inorder, rootIndex),
        buildTreePreIn(preorder+rootIndex+1, inorder+rootIndex+1, size-rootIndex-1)
    );
    return tree;
}

Node* buildTreePostIn(int* postorder, int* inorder, const int size) {
    if (!postorder || !inorder || !size) return NULL;
    
    int rootValue = postorder[size-1];
    int rootIndex = 0;  // index of root in the inorder sequence
    while (rootIndex < size && inorder[rootIndex] != rootValue)
        rootIndex++;

    Node* tree = newNode(
        rootValue,
        buildTreePostIn(postorder, inorder, rootIndex),
        buildTreePostIn(postorder+rootIndex, inorder+rootIndex+1, size-rootIndex-1)
    );
    return tree;
}

int main(void) {
    int numCases = 0;
    scanf("%d", &numCases);
    
    for (int n = 0; n < numCases; n++) {
        char order[18] = "";
        int numNodes = 0;
        scanf("%s %d", order, &numNodes);
        
        int* seq1 = (int*)malloc(sizeof(int) * numNodes);
        for (int i = 0; i < numNodes; i++) {
            scanf("%d", seq1 + i);
        }

        int* seq2 = (int*)malloc(sizeof(int) * numNodes);
        for (int i = 0; i < numNodes; i++) {
            scanf("%d", seq2 + i);
        }

        if (STREQ(order, "preorder-inorder")) {
            Node* tree = buildTreePreIn(seq1, seq2, numNodes);
            postorder(tree);
            printf("\n");
            freeNode(tree);
        } else if (STREQ(order, "postorder-inorder")) {
            Node* tree = buildTreePostIn(seq1, seq2, numNodes);
            preorder(tree);
            printf("\n");
            freeNode(tree);
        }
        free(seq1);
        free(seq2);
    }
    return 0;
}

