#include <stdio.h>
#include <string.h>

int main() {
    char stmt[10][20];
    int n, i;

    printf("Enter number of three address statements: ");
    scanf("%d", &n);

    printf("Enter the statements:\n");
    for (i = 0; i < n; i++) {
        scanf("%s", stmt[i]);
    }

    for (i = 0; i < n; i++) {
        char result[10], arg1[10], arg2[10], op;
        char *eq = strchr(stmt[i], '=');
        if (eq) {
            *eq = '\0';
            strcpy(result, stmt[i]);
            char *rhs = eq + 1;
            char *op_pos = NULL;

            if ((op_pos = strchr(rhs, '+'))) op = '+';
            else if ((op_pos = strchr(rhs, '-'))) op = '-';
            else if ((op_pos = strchr(rhs, '*'))) op = '*';
            else if ((op_pos = strchr(rhs, '/'))) op = '/';
            else op = '\0';

            if (op) {
                *op_pos = '\0';
                strcpy(arg1, rhs);
                strcpy(arg2, op_pos + 1);
                
                printf("\nLOAD %s\n", arg1);
                switch(op) {
                    case '+': printf("ADD %s\n", arg2); break;
                    case '-': printf("SUB %s\n", arg2); break;
                    case '*': printf("MUL %s\n", arg2); break;
                    case '/': printf("DIV %s\n", arg2); break;
                }
                printf("STORE %s\n", result);
            } else {
                strcpy(arg1, rhs);
                printf("\nLOAD %s\n", arg1);
                printf("STORE %s\n", result);
            }
        }
    }

    return 0;
}
