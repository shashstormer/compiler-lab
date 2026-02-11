%{
#include <stdio.h>
int lines = 0, spaces = 0, tabs = 0;
%}

%%
\n      { lines++; }
" "     { spaces++; }
\t      { tabs++; }
.       { }
%%

int yywrap() { return 1; }

int main() {
    printf("Enter text:\n");
    yylex();
    printf("Lines: %d\nSpaces: %d\nTabs: %d\n", lines, spaces, tabs);
    return 0;
}
