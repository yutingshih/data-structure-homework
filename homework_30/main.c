// Patricia Digital Search Tree

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define STREQ(a, b) (strcmp((a), (b)) == 0)
#define STRDUP(src) strcpy(malloc(strlen(src) + 1), src)

typedef struct Patricia {
    uint32_t bitNum;
    char* key;
    int32_t value;
    struct Patricia* left;
    struct Patricia* right;
} Patricia;

Patricia* newPatricia(const char* key, int32_t value, uint32_t bitNum) {
    Patricia* node = malloc(sizeof(Patricia));
    node->key = STRDUP(key);
    node->value = value;
    node->bitNum = bitNum;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void freePatricia(Patricia* root) {
    if (!root) return;
    if (root->key) free(root->key);
    if (root->left && root->left->bitNum > root->bitNum) {
        freePatricia(root->left);
    }
    if (root->right && root->right->bitNum > root->bitNum) {
        freePatricia(root->right);
    }
    free(root); root = NULL;
}

void printPatricia(Patricia* node) {
    if (!node) return;
    printf("%s", node->left ? node->left->key : "----");
    printf(" <- %s (%d) -> ", node->key, node->bitNum);
    printf("%s", node->right ? node->right->key : "----");
    printf("\n");
}

// Returns the ith bit of key with the given length
bool bit(const char* key, uint32_t i) {
    if (!key) {
        fprintf(stderr, "key does not exist\n");
        exit(EXIT_FAILURE);
    }
    if (i < 0 || i > strlen(key)) {
        fprintf(stderr, "index %u out of scope (1 ~ %ld)\n", i, strlen(key));
        exit(EXIT_FAILURE);
    }
    return i && key[i - 1] == '1';
}

// Find the index of the first different bit of two keys
// If two keys are identical, return 0
uint32_t firstDiffBit(const char* key1, const char* key2) {
    uint32_t len = strlen(key1);
    if (len != strlen(key2)) {
        fprintf(stderr, "compare two keys with diffrent length\n");
        exit(EXIT_FAILURE);
    }
    uint32_t i = 0;
    while (i < len && key1[i] == key2[i]) ++i;
    return i == len ? 0 : i + 1;
}

Patricia* search(Patricia* root, const char* key) {
    if (!root) return NULL;
    Patricia* curr = root;
    Patricia* next = curr->left;
    while (next->bitNum > curr->bitNum) {
        curr = next;
        next = bit(key, curr->bitNum) ? curr->right : curr->left;
    }
    return STREQ(next->key, key) ? next : NULL;
}

bool insert(Patricia** root, const char* key, const int32_t value) {
    if (!*root) {
        *root = newPatricia(key, value, 0);
        (*root)->left = *root;
        (*root)->right = NULL;
        return true;
    }

    Patricia* curr = *root;
    Patricia* next = curr->left;
    while (next->bitNum > curr->bitNum) {
        curr = next;
        next = bit(key, curr->bitNum) ? curr->right : curr->left;
    }

    uint32_t bitNum = firstDiffBit(next->key, key);
    if (!bitNum) return false;  // insert conflict

    Patricia* node = newPatricia(key, value, bitNum);
    curr = *root;
    next = curr->left;
    while (next->bitNum > curr->bitNum && --bitNum) {
        curr = next;
        next = bit(key, curr->bitNum) ? curr->right : curr->left;
    }

    if (bit(next->key, node->bitNum)) {
        node->left = node;
        node->right = next;
    } else {
        node->left = next;
        node->right = node;
    }
    if (bit(node->key, curr->bitNum)) {
        curr->right = node;
    } else {
        curr->left = node;
    }
    return true;
}

int32_t delete(Patricia** root, const char* key) {
    if (!*root) return -1;    // empty

    Patricia* prev = NULL;
    Patricia* curr = *root;
    Patricia* next = curr->left;
    while (next->bitNum > curr->bitNum) {
        prev = curr;
        curr = next;
        next = bit(key, curr->bitNum) ? curr->right: curr->left;
    }

    if (!STREQ(next->key, key)) return -1;    // not found

    int32_t value = next->value;
    if (*root == (*root)->left) {  // only one node
        *root = NULL;
        freePatricia(next);
        return value;
    }
    else if (next == curr) {  // deleted node has one self pointer
        if (prev->left == curr) {
            prev->left = curr->left == next ? curr->right : curr->left;
        } else if (prev->right == curr) {
            prev->right = curr->left == next ? curr->right : curr->left;
        } else {
            fprintf(stderr, "wrong traversal path\n");
            exit(EXIT_FAILURE);
        }
        next->left = next->right = NULL;
        freePatricia(next);
        return value;
    }
    else {  // deleted node has no self pointer
        Patricia* parent = prev;
        Patricia* child = curr->left != next ? curr->left : curr->right;

        prev = *root;
        Patricia* temp = prev->left;
        while (temp->bitNum > prev->bitNum) {
            prev = temp;
            temp = bit(curr->key, prev->bitNum) ? prev->right : prev->left;
        }  // now, temp == curr

    
        // copy data from curr to next
        free(next->key);
        next->key = curr->key;
        curr->key = NULL;
        next->value = curr->value;
        
        // let the pointer of prev pointing to curr point to next
        if (prev->left == curr) {
            prev->left = next;
        } else {
            prev->right = next;
        }

        // let the pointer of curr.parent pointing to curr point to curr.child
        if (parent->left == curr) {
            parent->left = child;
        } else {
            parent->right = child;
        }

        // delete curr node
        curr->left = curr->right = NULL;
        freePatricia(curr);
        return value;
    }
}

int32_t main(void) {
    uint32_t keyLen = 0;     // number of bits in every key
    scanf("%u", &keyLen);

    char cmd[7] = {0};
    char key[1001] = {0};
    int32_t value = 0;
    Patricia* root = NULL;

    for (int i = 0; i < 1000; i++) {
        scanf("%s", cmd);

        if (STREQ(cmd, "insert")) {
            scanf("%s %d", key, &value);
            bool inserted = insert(&root, key, value);
            if (inserted) printf("insert -> %d\n", value);
            else printf("insert -> conflict\n");
        }
        else if (STREQ(cmd, "search")) {
            scanf("%s", key);
            Patricia* node = search(root, key);
            if (node) printf("search -> %d\n", node->value);
            else printf("search -> not found\n");
        }
        else if (STREQ(cmd, "delete")) {
            scanf("%s", key);
            int32_t value = delete(&root, key);
            if (value >= 0) printf("delete -> %d\n", value);
            else printf("delete -> not found\n");
        }
        else if (STREQ(cmd, "quit")) break;
    }
    
    freePatricia(root);
    return 0;
}
