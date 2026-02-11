#include <stdio.h>
#include <string.h>

void helper(char *s1, char *s2, char *prefix) {
    int i=0;
    while(s1[i] && s2[i] && s1[i] == s2[i]) {
        prefix[i] = s1[i];
        i++;
    }
    prefix[i] = '\0';
}

int main() {
    char input[100], non_term;
    char parts[10][20];
    int part_count = 0;
    
    printf("Enter production (e.g., S->aAb|aAc): ");
    scanf("%s", input);
    
    non_term = input[0];
    int j=0;
    for(int i=3; i<strlen(input); i++) {
        if(input[i] == '|') {
            parts[part_count][j] = '\0';
            part_count++;
            j=0;
        } else {
            parts[part_count][j++] = input[i];
        }
    }
    parts[part_count][j] = '\0';
    part_count++;
    
    char prefix[20];
    if(part_count >= 2) {
        helper(parts[0], parts[1], prefix);
        int len = strlen(prefix);
        
        if(len > 0) {
            printf("%c->%s%c'\n", non_term, prefix, non_term);
            printf("%c'->", non_term);
            for(int i=0; i<part_count; i++) {
                if(strncmp(parts[i], prefix, len) == 0) {
                    if(strlen(parts[i]) == len) printf("epsilon");
                    else printf("%s", parts[i]+len);
                } else {
                    printf("%s", parts[i]);
                }
                if(i < part_count-1) printf("|");
            }
            printf("\n");
        } else {
            printf("No common prefix found.\n");
        }
    }
}
