// Random Walk Simulation

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NUM_CASES 20

typedef struct param {
    int row, col, ibug, jbug;
} param;

void simulate(const int row, const int col, int ib, int jb) {
    static const int imove[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
    static const int jmove[8] = {1, 1, 1, 0, -1, -1, -1, 0};

    int steps = 1;  // result
    int notEnd = row * col - 1;
    int** map = malloc(row * sizeof(int*));
    for (int i = 0; i < row; i++) {
        map[i] = calloc(col, sizeof(int));
    }
    map[ib][jb]++;

    int i, j;
    while (steps < 50000 && notEnd) {
        do {
            int dir = rand() % 8;
            i = ib + imove[dir];
            j = jb + jmove[dir];
        } while (i < 0 || i >= row || j < 0 || j >= col);
        
        ib = i; jb = j; steps++;
        notEnd -= !(map[ib][jb]++);
    }
    
    printf("%d\n", steps);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf("%4d", map[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < row; i++) {
        free(map[i]);
    }
    free(map);
}

int main(void) {
    srand(time(NULL));
    
    param prm[MAX_NUM_CASES] = {};
    int i = 0;

    char ch;
    int row, col, ibug, jbug, tmp;

    while (true) {
        scanf("%c", &ch);
        if (ch == 'a') {
            scanf("%d %d", &row, &col);
            ibug = row / 2;
            jbug = col / 2;
            prm[i++] = (param){row, col, ibug, jbug};
        } else if (ch == 'b') {
            scanf("%d", &tmp);
            if (tmp == 1) {
                row = col = 15;
                ibug = jbug = 10;
            } else {
                row = 39;
                col = 19;
                ibug = jbug = 1;
            }
            prm[i++] = (param){row, col, ibug, jbug};
        } else {
            break;
        }
        getchar();
    }

    for (int j = 0; j < i; j++){
        simulate(prm[j].row, prm[j].col, prm[j].ibug, prm[j].jbug);
    }
    return 0;
}
