%{
#include <stdio.h>
%}

%%
[a-zA-Z_][a-zA-Z0-9_]*   { printf("Valid Identifier: %s\n", yytext); }
.|\n                     { }
%%

int yywrap() { return 1; }

int main() {
    printf("Enter codes to check identifiers:\n");
    yylex();
    return 0;
}
