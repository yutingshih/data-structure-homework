// Prefix/Postfix Expression Conversion

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRLEN 30

void prefix_to_postfix(const char* prefix, char* postfix, size_t len) {
    int top = 0;
    char stack[len][len];
    char operand1[len];
    char operand2[len];
    char operator[len];

    for (int i = len - 1; i >= 0; i--) {
        switch (prefix[i]) {
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            case '^':
                strcpy(operand1, stack[--top]);
                strcpy(operand2, stack[--top]);
                strcpy(operator, (char []){prefix[i], '\0'});
                strcpy(stack[top++], strcat(strcat(operand1, operand2), operator));
                break;
            
            default:
                strcpy(stack[top++], (char []){prefix[i], '\0'});
                break;
        }
#ifdef DEBUG
        for (int i = 0; i < top; i++) {
            printf("stack[%d] = %s\n", i, stack[i]);
        }
        printf("\n");
#endif
    }
    strcpy(postfix, stack[--top]);
}

void postfix_to_prefix(const char* postfix, char* prefix, size_t len) {
    int top = 0;
    char stack[len][len];
    char operand1[len];
    char operand2[len];
    char operator[len];

    for (int i = 0; i < len; i++) {
        switch (postfix[i]) {
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            case '^':
                strcpy(operator, (char []){postfix[i], '\0'});
                strcpy(operand1, stack[--top]);
                strcpy(operand2, stack[--top]);
                strcpy(stack[top++], strcat(operator, strcat(operand2, operand1)));
                break;
            
            default:
                strcpy(stack[top++], (char []){postfix[i], '\0'});
                break;
        }
#ifdef DEBUG
        for (int i = 0; i < top; i++) {
            printf("stack[%d] = %s\n", i, stack[i]);
        }
        printf("\n");
#endif
    }
    strcpy(prefix, stack[--top]);
}

int main(void) {
    char expr1[STRLEN] = "";
    char expr2[STRLEN] = "";
    scanf("%s ", expr1);
    scanf("%s ", expr2);

    char res1[STRLEN];
    char res2[STRLEN];
    prefix_to_postfix(expr1, res1, strlen(expr1));
    postfix_to_prefix(expr2, res2, strlen(expr2));

    printf("%s\n", res1);
    printf("%s\n", res2);
    return 0;
}
