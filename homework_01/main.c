// Fibonacci Sequence

#include <stdio.h>
#include <string.h>

#define STREQ(str1, str2) (strcmp((str1), (str2)) == 0)

unsigned int fib_recursive(unsigned int n) {
    if (n <= 1) return n;
    return fib_recursive(n - 1) + fib_recursive(n - 2);
}

unsigned int fib_iterative(unsigned int n) {
    unsigned int prev = 0, curr = 0, next = 1;
    for (unsigned int i = 0; i < n; ++i) {
        prev = curr;
        curr = next;
        next = curr + prev;
    }
    return curr;
}

int main(void) {
    int num;
    scanf("%d", &num);
    
    char type[10] = {'\0'};
    int term = 0;

    for (int i = 0; i < num; i++) {
        scanf("%s %d", type, &term);
        
        if (STREQ(type, "recursive")) {
            printf("%d\n", fib_recursive(term));
        } else if (STREQ(type, "iterative")) {
            printf("%d\n", fib_iterative(term));
        }
    }

    return 0;
}
