// Nonrecursive Preorder, inorder & Postorder Traversal

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct TreeNode {
    int data;   // for root node, data = number of decendents of the root
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

typedef enum { L = 1, R = 0 } Direction;
typedef struct StackNode {
    TreeNode* node;
    Direction nextDir;  // indicate which child of node to be inserted next
} StackNode;

TreeNode* buildTree(const int* list, const int n) {
    if (!n) return NULL;

    StackNode* stack = malloc(sizeof(StackNode) * n * 2);
    int top = 0;

    TreeNode* root = malloc(sizeof(TreeNode));
    *root = (TreeNode){0, NULL, NULL};
    stack[top++] = (StackNode){root, L};

    for (int i = 0; i < n; i++) {
        StackNode parent = stack[--top];

        if (list[i] >= 0) {
            TreeNode* newNode = malloc(sizeof(TreeNode));
            *newNode = (TreeNode){list[i], NULL, NULL};
            root->data++;   // increase number of tree nodes

            if (parent.nextDir == L) {
                parent.node->left = newNode;
            } else {
                parent.node->right = newNode;
            }

            stack[top++] = (StackNode){newNode, R};
            stack[top++] = (StackNode){newNode, L};
        }
    }
    
    free(stack); stack = NULL;
    return root;
}

void freeTree(TreeNode* root) {
    if (!root) return;
    if (root->left) freeTree(root->left);
    if (root->right) freeTree(root->right);
    free(root); root = NULL;
}

void preorder(const TreeNode* root) {
    if (!root || !root->left) return;

    if (root->right) {
        fprintf(stderr, "invalid tree\n");
        exit(EXIT_FAILURE);
    }

    TreeNode* stack[root->data];
    int top = 0;

    stack[top++] = root->left;
    while (top) {
        TreeNode* curr = stack[--top];
        printf("%d ", curr->data);
        if (curr->right) stack[top++] = curr->right;
        if (curr->left) stack[top++] = curr->left;
    }
}

void inorder(const TreeNode* root) {
    if (!root || !root->left) return;

    if (root->right) {
        fprintf(stderr, "invalid tree\n");
        exit(EXIT_FAILURE);
    }

    TreeNode* stack[root->data];
    int top = 0;

    TreeNode* curr = root->left;
    while (curr || top) {
        if (curr) {
            stack[top++] = curr;
            curr = curr->left;
        } else {
            curr = stack[--top];
            printf("%d ", curr->data);
            curr = curr->right;
        }
    }
}

void postorder(const TreeNode* root) {
    if (!root || !root->left) return;

    if (root->right) {
        fprintf(stderr, "invalid tree\n");
        exit(EXIT_FAILURE);
    }

    TreeNode* stack1[root->data];
    TreeNode* stack2[root->data];
    int top1 = 0;
    int top2 = 0;

    stack1[top1++] = root->left;
    
    while (top1) {
        TreeNode* curr = stack1[--top1];
        stack2[top2++] = curr;

        if (curr->left) stack1[top1++] = curr->left;
        if (curr->right) stack1[top1++] = curr->right;
    }

    while (top2) {
        TreeNode* curr = stack2[--top2];
        printf("%d ", curr->data);
    }
}

int main(void) {
    int n;
    scanf("%d", &n);

    int* list = (int*)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        scanf("%d", list + i);
    }

    TreeNode* tree = buildTree(list, n);

    preorder(tree);
    printf("\n");

    postorder(tree);
    printf("\n");

    free(list); list = NULL;
    freeTree(tree);
    return 0;
}
