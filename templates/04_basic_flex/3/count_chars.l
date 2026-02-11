%{
#include <stdio.h>
int count = 0;
%}

%%
.       { count++; }
\n      { count++; }
%%

int yywrap() { return 1; }

int main() {
    printf("Enter text:\n");
    yylex();
    printf("Total Characters: %d\n", count);
    return 0;
}
