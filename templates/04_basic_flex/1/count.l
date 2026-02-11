%{
#include <stdio.h>
int chars = 0;
int words = 0;
int lines = 0;
%}

%%
[a-zA-Z0-9]+  { words++; chars += yyleng; }
\n            { lines++; chars++; }
.             { chars++; }
%%

int yywrap() { return 1; }

int main() {
    printf("Enter text (Ctrl+D to end):\n");
    yylex();
    printf("Lines: %d\nWords: %d\nChars: %d\n", lines, words, chars);
    return 0;
}
