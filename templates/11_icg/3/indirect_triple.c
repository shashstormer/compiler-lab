#include<stdio.h>
#include<string.h>

struct quad {
    char op[5];
    char arg1[10];
    char arg2[10];
    char result[10];
} q[20];

int n = 0;

int main() {
    printf("Enter number of expressions: ");
    int num;
    scanf("%d", &num);
    
    printf("\nOP\tARG1\tARG2\tRESULT\n");
    return 0;
}
