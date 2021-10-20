// Fast Transpose of Sparse Matrix

#include <stdio.h>
#define MAX_SIZE 100

typedef struct {
    int row;
    int col;
    int val;
} term;

// The sparse matrix is represented by an array of term
// The 0th val of the array is the number of elements of matrix

void print_matrix(const term m[]) {
    for (int i = 0; i <= m[0].val; i++) {
        printf("%d %d %d\n", m[i].row, m[i].col, m[i].val);
    }
}

void fast_transpose(const term a[], term b[]) {
    b[0] = (term){a[0].col, a[0].row, a[0].val};
    if (!a[0].val) return;
    
    // index: starting position of each row of b
    int index[MAX_SIZE + 1] = {1}; // first row starts from a[1]
    
    // count how many terms of a will be filled in each row of b
    for (int i = 1; i <= a[0].val; i++) {
        index[a[i].col + 1]++;
    }

    // calculate the starting index of each row via prefix sum
    for (int i = 1; i <= b[0].row; i++) {
        index[i] += index[i - 1];
    }

    int j = 1;
    for (int i = 1; i <= a[0].val; i++) {
        j = index[a[i].col]++;
        b[j] = (term){a[i].col, a[i].row, a[i].val};
    }
}

int main(void) {
    term a[MAX_SIZE] = {};
    scanf("%d %d %d", &a[0].row, &a[0].col, &a[0].val);
    for (int i = 1; i <= a[0].val; i++) {
        scanf("%d %d %d", &a[i].row, &a[i].col, &a[i].val);
    }

    term b[MAX_SIZE] = {};
    fast_transpose(a, b);

    print_matrix(b);
    return 0;
}

