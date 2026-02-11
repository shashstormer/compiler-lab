%{
#include <stdio.h>
%}

%%
if|else|while|int|float|return|void|char|double|struct|case|switch|break|continue  { printf("Keyword: %s\n", yytext); }
.|\n    { }
%%

int yywrap() { return 1; }

int main() {
    printf("Enter C code:\n");
    yylex();
    return 0;
}
