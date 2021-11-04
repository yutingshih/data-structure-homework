// Height Union

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define STREQ(a, b) (strcmp((a), (b)) == 0)
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int find(int* parent, int i) {
    int root, trail, lead;
    for (root = i; parent[root] >= 0; root = parent[root]);
    for (trail = i; trail != root; trail = lead) {
        lead = parent[trail];
        parent[trail] = root;
    }
    return root;
}

void heightUnion(int* parent, int i, int j) {
    i = find(parent, i);
    j = find(parent, j);

    if (i == j) return;

    if (parent[i] > parent[j]) {
        parent[j] = MIN(parent[j], parent[i] - 1);
        parent[i] = j;
    } else {
        parent[i] = MIN(parent[i], parent[j] - 1);
        parent[j] = i;
    }
}

bool same(int* set, int i, int j) {
    return find(set, i) == find(set, j);
}

int main(void) {
    int numCases = 0;
    scanf("%d", &numCases);
    for (int n = 0; n < numCases; n++) {
        int numElem = 0, numOps = 0;
        scanf("%d %d", &numElem, &numOps);
        
        int set[numElem];
        for (int i = 0; i < numElem; i++)
            set[i] = -1;

        for (int p = 0; p < numOps; p++) {
            char op[6] = "";
            int elem1, elem2;
            scanf("%s", op);
            if (STREQ(op, "union")) {
                scanf("%d %d", &elem1, &elem2);
                heightUnion(set, elem1, elem2);
            } else if (STREQ(op, "find")) {
                scanf("%d", &elem1);
                printf("%d\n", find(set, elem1));
            } else if (STREQ(op, "same")) {
                scanf("%d %d", &elem1, &elem2);
                printf("%s\n", same(set, elem1, elem2) ? "true" : "false");
            }
        }
    }
    return 0;
}

