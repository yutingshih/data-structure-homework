// Maze

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int row, col;
} offset;

const offset move[8] = {
    [0] = {-1, 0},
    [1] = {-1, 1},
    [2] = {0, 1},
    [3] = {1, 1},
    [4] = {1, 0},
    [5] = {1, -1},
    [6] = {0, -1},
    [7] = {-1, -1}
};

typedef struct {
    int row, col, dir;
} StackNode;

typedef enum {
    WALL = 1,
    PATH = 0,
    WALKED = 2,
} MazeGrid;

#ifdef DEBUG
void print_maze(MazeGrid** maze, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf("%2s", maze[i][j] == WALL ? "＠": maze[i][j] == PATH ? " " : "。");
        }
        printf("\n");
    }
}
#endif

bool search_path(MazeGrid** maze, int num_row, int num_col, StackNode stack[], int* top) {
    StackNode curr = (StackNode){.row = 1, .col = 1, .dir = 0};
    maze[curr.row][curr.col] = WALKED;
    stack[(*top)++] = curr;

    bool found = false;
    while (*top && !found) {
        while (++curr.dir < 8 && !found) {
            StackNode next = (StackNode){
                .row = curr.row + move[curr.dir].row,
                .col = curr.col + move[curr.dir].col,
            };

            if (maze[next.row][next.col] == PATH) {
                maze[next.row][next.col] = WALKED;
                stack[(*top)++] = next;
                curr = next;
            }

            found = (curr.row == num_row - 2) && (curr.col == num_col - 2);
        }
        if (curr.dir == 8) --(*top);
        else curr = stack[(*top) - 1];
    }
    return found;
}

int main(void) {
    int H, W;
    scanf("%d %d ", &H, &W);
    const int ROW = H + 2;
    const int COL = W + 2;

    MazeGrid* maze[ROW];
    for (int i = 0; i < ROW; i++) {
        maze[i] = (MazeGrid*)malloc(sizeof(MazeGrid) * COL);
        maze[i][0] = maze[i][COL - 1] = WALL;
    }
    for (int i = 0; i < COL; i++) {
        maze[0][i] = maze[ROW - 1][i] = WALL;
    }
    for (int i = 1; i <= H; i++) {
        for (int j = 1; j <= W; j++) {
            int tmp;
            scanf("%d ", &tmp);
            maze[i][j] = tmp;
        }
    }

    int top = 0;
    StackNode stack[H * W];
    bool found = search_path(maze, ROW, COL, stack, &top);

#ifdef DEBUG
    print_maze(maze, ROW, COL);
#endif

    if (!found) {
        printf("no answer\n");
    } else {
        for (int i = 0; i < top; i++) {
            printf("%d %d\n", stack[i].row - 1, stack[i].col - 1);
        }
    }

    for (int i = 0; i < ROW; i++) {
        free(maze[i]);
    }
    return 0;
}
