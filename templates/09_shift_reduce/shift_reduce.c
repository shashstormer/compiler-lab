#include<stdio.h>
#include<string.h>

#define MAX 20

int num_prod;
char lhs[MAX];
char rhs[MAX][MAX];
char stack[MAX], input[MAX];
int top = -1;

void push(char c) {
    stack[++top] = c;
    stack[top + 1] = '\0';
}

void display(char *inp, char *action) {
    printf("%-20s%-20s%s\n", stack, inp, action);
}

int check_reduce(char *inp) {
    int i, j, k, slen;
    char action[50];

    for(i = 0; i < num_prod; i++) {
        int rlen = strlen(rhs[i]);
        slen = strlen(stack);
        if(rlen > slen) continue;

        /* Check if top of stack matches rhs[i] */
        int match = 1;
        for(j = 0; j < rlen; j++) {
            if(stack[slen - rlen + j] != rhs[i][j]) {
                match = 0;
                break;
            }
        }
        if(match) {
            sprintf(action, "Reduce %c->%s", lhs[i], rhs[i]);
            display(inp, action);
            /* Pop rlen symbols and push lhs */
            top = slen - rlen - 1;
            push(lhs[i]);
            return 1;
        }
    }
    return 0;
}

int main() {
    int i, ip;
    char prod[MAX], action[50];

    printf("Enter number of productions: ");
    scanf("%d", &num_prod);
    printf("Enter productions:\n");
    for(i = 0; i < num_prod; i++) {
        scanf("%s", prod);
        lhs[i] = prod[0];
        strcpy(rhs[i], prod + 3); /* skip A-> */
    }

    printf("Enter input string: ");
    scanf("%s", input);
    strcat(input, "$");

    printf("\n%-20s%-20s%s\n", "Stack", "Input", "Action");
    printf("------------------------------------------------\n");

    strcpy(stack, "$");
    top = 0;
    ip = 0;

    while(1) {
        /* Check acceptance */
        if(strcmp(stack + 1, "") != 0 && stack[1] == lhs[0] &&
           strlen(stack) == 2 && input[ip] == '$') {
            display(&input[ip], "Accept");
            break;
        }

        /* Try reduce */
        if(top > 0 && check_reduce(&input[ip]))
            continue;

        /* Shift */
        if(input[ip] != '$') {
            sprintf(action, "Shift %c", input[ip]);
            display(&input[ip], action);
            push(input[ip]);
            ip++;
        } else {
            display(&input[ip], "Reject");
            break;
        }
    }

    return 0;
}
