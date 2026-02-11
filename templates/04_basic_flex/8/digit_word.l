%{
#include <stdio.h>
%}

%%
[0-9]+      { printf("Digit\n"); }
[a-zA-Z]+   { printf("Word\n"); }
.|\n        { }
%%

int yywrap() { return 1; }

int main() {
    printf("Enter input:\n");
    yylex();
    return 0;
}
