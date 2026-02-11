#include<stdio.h>
#include<string.h>

int main() {
    char ic[10][20], op[10], arg1[10], arg2[10], result[10];
    int n, i;
    
    printf("Enter number of intermediate code lines: ");
    scanf("%d", &n);
    
    printf("Enter 3-address code (e.g., t1=a+b):\n");
    for(i=0; i<n; i++)
        scanf("%s", ic[i]);
        
    printf("\nTarget Code (Assembly):\n");
    for(i=0; i<n; i++) {
        printf("MOV R0, ...\n");
    }
    
    return 0;
}
