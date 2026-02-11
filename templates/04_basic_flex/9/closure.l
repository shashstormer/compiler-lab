%{
#include <stdio.h>
%}

%%
[a-zA-Z]+   { printf("Positive Closure detected: %s\n", yytext); }
.|\n        { }
%%

int yywrap() { return 1; }

int main() {
    printf("Enter input (detects a+):\n");
    yylex();
    return 0;
}
