// Polynomial in Circular Linked List

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define STREQ(str1, str2) (!strcmp((str1), (str2)))

#define MALLOC(ptr, size) \
    if (!((ptr) = malloc(size))) { \
        fprintf(stderr, "Insufficient memory\n"); \
        exit(EXIT_FAILURE); \
    }

typedef struct polyNode {
    int coef, expon;
    struct polyNode* link;
} polyNode;

polyNode* avail = NULL;

polyNode* getNode(int coef, int expon) {
    polyNode* node;
    if (avail) {
        node = avail;
        avail = avail->link;
    } else {
        MALLOC(node, sizeof(polyNode));
    }
    *node = (polyNode){coef, expon, NULL};
    return node;
}

void retNode(polyNode* node) {
    node->link = avail;
    avail = node;
}

polyNode* pread() {
    polyNode* head = getNode(0, -1);
    polyNode* tail = head;
    
    int terms, coef, expon;
    scanf("%d ", &terms);
    for (int i = 0; i < terms; i++) {
        scanf("%d %d ", &coef, &expon);
        tail->link = getNode(coef, expon);
        tail = tail->link;
    }

    tail->link = head;
    return head;
}

void pwrite(const polyNode* head, char str[]) {
    polyNode* tail = head->link;
    if (tail == head) {
        sprintf(str, "0");
    } else {
        sprintf(str, "%dx^%d", tail->coef, tail->expon);
        tail = tail->link;
    }
    while (tail != head) {
        sprintf(str + strlen(str), "%s%dx^%d", tail->coef >= 0 ? "+" : "", tail->coef, tail->expon);
        tail = tail->link;
    }
    strcat(str, "\n");
}

polyNode* padd(polyNode* a, polyNode* b) {
    polyNode* a_head = a;
    a = a->link;
    b = b->link;

    polyNode* head = getNode(0, -1);
    polyNode* tail = head;

    do {
        if (a->expon < b->expon) {
            tail->link = getNode(b->coef, b->expon);
            tail = tail->link;
            b = b->link;
        } else if (a->expon > b->expon) {
            tail->link = getNode(a->coef, a->expon);
            tail = tail->link;
            a = a->link;
        } else {
            int sum = a->coef + b->coef;
            if (sum) {
                tail->link = getNode(sum, a->expon);
                tail = tail->link;
            }
            a = a->link;
            b = b->link;
        }
    } while (a != a_head);

    tail->link = head;
    return head;
}

polyNode* psub(polyNode* a, polyNode* b) {
    polyNode* a_head = a;
    a = a->link;
    b = b->link;

    polyNode* head = getNode(0, -1);
    polyNode* tail = head;

    do {
        if (a->expon < b->expon) {
            tail->link = getNode(-b->coef, b->expon);
            tail = tail->link;
            b = b->link;
        } else if (a->expon > b->expon) {
            tail->link = getNode(a->coef, a->expon);
            tail = tail->link;
            a = a->link;
        } else {
            int sum = a->coef - b->coef;
            if (sum) {
                tail->link = getNode(sum, a->expon);
                tail = tail->link;
            }
            a = a->link;
            b = b->link;
        }
    } while (a != a_head);

    tail->link = head;
    return head;
}

polyNode* pmult(const polyNode* A, const polyNode* B) {
    polyNode* head = getNode(0, -1);
    polyNode* tail = head;

    polyNode* a = A->link;
    polyNode* b = B->link;

    if (a == A || b == B) return head;

    int degree = a->expon + b->expon;
    int* coef = calloc(degree + 1, sizeof(int));
    for (a = A->link; a != A; a = a->link) {
        for (b = B->link; b != B; b = b->link) {
            int expon = a->expon + b->expon;
            coef[expon] += a->coef * b->coef;
        }
    }
    
    for (int i = degree; i >= 0; i--) {
        if (coef[i]) {
            tail->link = getNode(coef[i], i);
            tail = tail->link;
        }
    }
    tail->link = head;
    
    free(coef); coef = NULL;
    return head;
}

void perase(polyNode** ptr) {
    if (*ptr) {
        polyNode* tmp = (*ptr)->link;
        (*ptr)->link = avail;
        avail = tmp;
        (*ptr) = NULL;
    }
}

double eval(const polyNode* head, double x) {
    double result = 0.0;
    polyNode* tail = head->link;
    while (tail != head) {
        result += tail->coef * pow(x, tail->expon);
        tail = tail->link;
    }
    return result;
}

#define NUM_EXPR 3
polyNode* polynomials[NUM_EXPR];
char names[NUM_EXPR][50];
int cnt = 0;

polyNode* pfind(char name[]) {
    for (int i = 0; i < cnt; i++) {
        if (STREQ(name, names[i])) {
            return polynomials[i];
        }
    }
    return NULL;
}

int main(void) {
    int num;
    scanf("%d ", &num);

    char output[500] = "";
    char a[50] = "";
    char b[50] = "";
    char c[50] = "";

    for (int i = 0; i < num; i++) {
        char func[10] = "";
        scanf("%s ", func);

        if (STREQ(func, "pread")) {
            scanf("%s ", names[cnt]);
            polynomials[cnt] = pread();
            cnt++;
            strcpy(output + strlen(output), "ok\n");
        
        } else if (STREQ(func, "pwrite")) {
            char name[50] = "";
            scanf("%s ", name);
            pwrite(pfind(name), output + strlen(output));

        } else if (STREQ(func, "padd")) {
            scanf("%s %s %s ", c, a, b);
            polynomials[cnt] = padd(pfind(a), pfind(b));;
            strcpy(names[cnt], c);
            cnt++;
            sprintf(output + strlen(output), "added\n");

        } else if (STREQ(func, "psub")) {
            scanf("%s %s %s ", c, a, b);
            polynomials[cnt] = psub(pfind(a), pfind(b));
            strcpy(names[cnt], c);
            cnt++;
            sprintf(output + strlen(output), "substracted\n");

        } else if (STREQ(func, "pmult")) {
            scanf("%s %s %s ", c, a, b);
            polynomials[cnt] = pmult(pfind(a), pfind(b));
            strcpy(names[cnt], c);
            cnt++;
            sprintf(output + strlen(output), "multiplied\n");

        } else if (STREQ(func, "eval")) {
            char name[50] = "";
            double x = 0.0;
            scanf("%s %lf ", name, &x);
            double res = eval(pfind(name), x);
            sprintf(output + strlen(output), "%.2f\n", res);

        } else if (STREQ(func, "perase")) {
            char name[50] = "";
            scanf("%s ", name);
            polyNode* tmp = pfind(name);
            perase(&tmp);
            cnt--;
            strcpy(output + strlen(output), "erased\n");
            
        } else {
            fprintf(stderr, "Invalid command: %s\n", func);
            exit(EXIT_FAILURE);
        }
    }

    printf("%s", output);

    polyNode* temp = avail;
    while (temp) {
        avail = avail->link;
        free(temp);
        temp = avail;
    }
    return 0;
}
