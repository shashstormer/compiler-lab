%{
#include <stdio.h>
int tokens = 0;
%}

%%
[a-zA-Z0-9]+    { tokens++; }
.               { tokens++; }
\n              { }
%%

int yywrap() { return 1; }

int main() {
    printf("Enter input:\n");
    yylex();
    printf("Total Tokens: %d\n", tokens);
    return 0;
}
