%{
#include <stdio.h>
%}

%%
[A-Z]   { printf("%s is Uppercase\n", yytext); }
[a-z]   { printf("%s is Lowercase\n", yytext); }
.|\n    { }
%%

int yywrap() { return 1; }

int main() {
    printf("Enter text:\n");
    yylex();
    return 0;
}
