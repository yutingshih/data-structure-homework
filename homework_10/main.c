// Swap Binary Tree

#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int data;   // for root, data is the number of decendents
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

typedef enum Direction { L = 1, R = 0 } Direction;
typedef struct StackNode {
    TreeNode* node;
    Direction nextDir;
} StackNode;

TreeNode* buildTree(const int* list, const int n) {
    TreeNode* root = calloc(1, sizeof(TreeNode));
    if (!list || !n) return root;

    StackNode* stack = malloc(sizeof(StackNode) * n * 2);
    int top = 0;
    stack[top++] = (StackNode){root, L};

    for (int i = 0; i < n; i++) {
        StackNode parent = stack[--top];

        if (list[i] >= 0) {
            TreeNode* newNode = malloc(sizeof(TreeNode));
            *newNode = (TreeNode){list[i], NULL, NULL};
            root->data++;

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

    TreeNode* curr = root->left;
    stack[top++] = curr;

    while (top) {
        curr = stack[--top];
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

    TreeNode* curr = root->left;
    stack1[top1++] = curr;
    
    while (top1) {
        curr = stack1[--top1];
        stack2[top2++] = curr;

        if (curr->left) stack1[top1++] = curr->left;
        if (curr->right) stack1[top1++] = curr->right;
    }
    
    while (top2) {
        curr = stack2[--top2];
        printf("%d ", curr->data);
    }
}

void _swapTree(TreeNode* root) {
    if (!root) return;
    TreeNode* temp = root->left;
    root->left = root->right;
    root->right = temp;
    if (root->left) _swapTree(root->left);
    if (root->right) _swapTree(root->right);
}
void swapTree(TreeNode* root) {
    if (!root || !root->left) return;
    if (root->right) {
        fprintf(stderr, "invalid tree\n");
        exit(EXIT_FAILURE);
    }
    _swapTree(root->left);
}

int main(void) {
    int n;
    scanf("%d", &n);

    int* list = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        scanf("%d", list + i);
    }

    TreeNode* tree = buildTree(list, n);
    swapTree(tree);

    preorder(tree);
    printf("\n");

    inorder(tree);
    printf("\n");

    free(list); list = NULL;
    return 0;
}
