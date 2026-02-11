#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char stack[20], a[20], action[20];
int input_len, stack_top = 0;

void output() {
    printf("\n$%s\t%s\t%s", stack, a, action);
}

void check() {
    strcpy(action, "REDUCE");
}

int main() {
    printf("Enter the input string: ");
    scanf("%s", a);
    input_len = strlen(a);
    
    printf("\nSTACK\tINPUT\tACTION");
    
    return 0;
}
