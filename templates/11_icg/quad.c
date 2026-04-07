#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct triple {
    char op[10];
    char arg1[10];
    char arg2[10];
} triples[10];

struct quadruple {
    char op[10];
    char arg1[10];
    char arg2[10];
    char result[10];
} quads[10];

int main() {
    int n, i;
    char stmt[20];

    printf("Enter number of statements: ");
    scanf("%d", &n);

    printf("Enter the statements:\n");
    for (i = 0; i < n; i++) {
        scanf("%s", stmt);
        char *eq = strchr(stmt, '=');
        if (eq) {
            *eq = '\0';
            strcpy(quads[i].result, stmt);
            char *rhs = eq + 1;

            char *op_pos = NULL;
            if ((op_pos = strchr(rhs, '+'))) strcpy(quads[i].op, "+");
            else if ((op_pos = strchr(rhs, '-'))) strcpy(quads[i].op, "-");
            else if ((op_pos = strchr(rhs, '*'))) strcpy(quads[i].op, "*");
            else if ((op_pos = strchr(rhs, '/'))) strcpy(quads[i].op, "/");

            if (op_pos) {
                *op_pos = '\0';
                strcpy(quads[i].arg1, rhs);
                strcpy(quads[i].arg2, op_pos + 1);
            } else {
                strcpy(quads[i].op, "=");
                strcpy(quads[i].arg1, rhs);
                strcpy(quads[i].arg2, " ");
            }
            
            strcpy(triples[i].op, quads[i].op);
            strcpy(triples[i].arg1, quads[i].arg1);
            strcpy(triples[i].arg2, quads[i].arg2);
        }
    }

    printf("\nQuadruple Representation\n");
    printf("Op\tArg1\tArg2\tResult\n");
    for (i = 0; i < n; i++) {
        printf("%s\t%s\t%s\t%s\n", quads[i].op, quads[i].arg1, quads[i].arg2, quads[i].result);
    }

    printf("\nTriple Representation\n");
    printf("Index\tOp\tArg1\tArg2\n");
    for (i = 0; i < n; i++) {
        printf("%d\t%s\t%s\t%s\n", i, triples[i].op, triples[i].arg1, triples[i].arg2);
    }

    printf("\nIndirect Triple Representation\n");
    printf("Pointer\tIndex\n");
    for (i = 0; i < n; i++) {
        printf("%d\t%d\n", i, i);
    }

    return 0;
}
