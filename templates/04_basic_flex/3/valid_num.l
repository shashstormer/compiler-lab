%{
#include <stdio.h>
%}

DIGIT [0-9]

%%
{DIGIT}+                  { printf("Valid Integer\n"); }
{DIGIT}+"."{DIGIT}+       { printf("Valid Float\n"); }
.*                        { printf("Invalid Number\n"); }
%%

int yywrap() { return 1; }

int main() {
    printf("Enter number to check (Integer/Float):\n");
    yylex();
    return 0;
}
