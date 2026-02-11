%{
#include <stdio.h>
#include <string.h>
char target[50];
int count = 0;
%}

%%
[a-zA-Z]+ { if(strcmp(yytext, target) == 0) count++; }
.|\n      { }
%%

int yywrap() { return 1; }

int main() {
    printf("Enter word to match: ");
    scanf("%s", target);
    printf("Enter text:\n");
    yylex();
    printf("Frequency: %d\n", count);
    return 0;
}
