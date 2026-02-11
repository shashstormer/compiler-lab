%{
#include <stdio.h>
int lines = 0;
int chars = 0;
%}

%%
\n      { lines++; chars++; }
.       { chars++; }
%%

int yywrap() { return 1; }

int main() {
    printf("Enter text:\n");
    yylex();
    printf("Lines: %d\nCharacters: %d\n", lines, chars);
    return 0;
}
