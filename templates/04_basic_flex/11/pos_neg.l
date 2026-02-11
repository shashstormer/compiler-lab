%{
#include <stdio.h>
int pos = 0, neg = 0;
%}

%%
"-"[0-9]+   { neg++; printf("Negative: %s\n", yytext); }
[0-9]+      { pos++; printf("Positive: %s\n", yytext); }
.|\n        { }
%%

int yywrap() { return 1; }

int main() {
    printf("Enter numbers:\n");
    yylex();
    printf("Positive count: %d\nNegative count: %d\n", pos, neg);
    return 0;
}
