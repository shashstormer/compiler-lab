#include<stdio.h>
#include<ctype.h>
#include<string.h>

void follow(char c);
void first(char c);

int count, n = 0;
char calc_first[10][100];
char calc_follow[10][100];
int m = 0;
char production[10][10];
char f[10], first_set[10];
int k;
char ck;
int e;

int main(int argc, char **argv) {
    int i, z;
    char c, ch;
    
    printf("Enter the number of productions:\n");
    scanf("%d", &count);
    
    printf("Enter the productions (e.g. E=TR):\n");
    for(i=0; i<count; i++)
        scanf("%s%c", production[i], &ch);
        
    do {
        m = 0;
        printf("Enter the element to calculate First & Follow: ");
        scanf("%c", &c);
        
        first(c);
        printf("\nFirst(%c) = { ", c);
        for(i=0; i<m; i++)
            printf("%c ", f[i]);
        printf("}\n");
        
        strcpy(f, " ");
        m = 0;
        follow(c);
        printf("Follow(%c) = { ", c);
        for(i=0; i<m; i++)
            printf("%c ", f[i]);
        printf("}\n");
        
        printf("Continue(0/1)? ");
        scanf("%d%c", &z, &ch);
    } while(z == 1);
    
    return 0;
}

void first(char c) {
    int j;
    if(!(isupper(c)))
        f[m++] = c;
    for(j=0; j<count; j++) {
        if(production[j][0] == c) {
            if(production[j][2] == '$')
                follow(production[j][0]);
            else if(islower(production[j][2]))
                f[m++] = production[j][2];
            else
                first(production[j][2]);
        }
    }
}

void follow(char c) {
    if(production[0][0] == c)
        f[m++] = '$';
    for(int i=0; i<10; i++) {
        for(int j=2; j<10; j++) {
            if(production[i][j] == c) {
                if(production[i][j+1] != '\0')
                    first(production[i][j+1]);
                if(production[i][j+1] == '\0' && c != production[i][0])
                    follow(production[i][0]);
            }
        }
    }
}
