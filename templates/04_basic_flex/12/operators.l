%{
#include <stdio.h>
%}

%%
"+"|"-"|"*"|"/"|"%"|"="|"<"|">"  { printf("Operator: %s\n", yytext); }
.|\n                             { }
%%

int yywrap() { return 1; }

int main() {
    printf("Enter input:\n");
    yylex();
    return 0;
}
