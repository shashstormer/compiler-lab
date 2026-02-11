%{
#include <stdio.h>
%}

%%
[0-9]+(\.[0-9]+)?   { printf("Valid Number: %s\n", yytext); }
.|\n                { }
%%

int yywrap() { return 1; }

int main() {
    printf("Enter tokens:\n");
    yylex();
    return 0;
}
