#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    char input[100], non_term;
    char *parts[10];
    int part_count = 0;
    
    printf("Enter production (e.g., E->E+T|T): ");
    scanf("%s", input);
    
    non_term = input[0];
    
    char *token = strtok(input + 3, "|");
    while(token != NULL) {
        parts[part_count++] = token;
        token = strtok(NULL, "|");
    }
    
    int recursive = 0;
    for(int i=0; i<part_count; i++) {
        if(parts[i][0] == non_term) {
            recursive = 1;
            break;
        }
    }
    
    if(!recursive) {
        printf("No left recursion\n");
        return 0;
    }
    
    printf("%c->", non_term);
    int first = 1;
    for(int i=0; i<part_count; i++) {
        if(parts[i][0] != non_term) {
            if(!first) printf("|");
            printf("%s%c'", parts[i], non_term);
            first = 0;
        }
    }
    printf("\n");
    
    printf("%c'->", non_term);
    first = 1;
    for(int i=0; i<part_count; i++) {
        if(parts[i][0] == non_term) {
            if(!first) printf("|");
            printf("%s%c'", parts[i]+1, non_term);
            first = 0;
        }
    }
    printf("|epsilon\n");
    
    return 0;
}
