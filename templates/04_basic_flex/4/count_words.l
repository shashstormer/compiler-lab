%{
#include <stdio.h>
int words = 0;
%}

%%
[a-zA-Z0-9]+ { words++; }
.|\n        { }
%%

int yywrap() { return 1; }

int main() {
    printf("Enter text:\n");
    yylex();
    printf("Total Words: %d\n", words);
    return 0;
}
