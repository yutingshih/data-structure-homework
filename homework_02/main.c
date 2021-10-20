// Tower of Hanoi

#include <stdio.h>

void move(int n, int from, int to) {
    printf("%d %d %d\n", n, from, to);
}

void hanoi(int n, int src, int mid, int dst) {
    if (n == 1) {
        move(n, src, dst);
    } else {
        hanoi(n - 1, src, dst, mid);
        move(n, src, dst);
        hanoi(n - 1, mid, src, dst);
    }
}

int main(void) {
    int num = 0;
    scanf("%d", &num);
    hanoi(num, 1, 2, 3);
    return 0;
}
