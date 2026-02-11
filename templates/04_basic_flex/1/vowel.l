%{
#include <stdio.h>
%}

%%
[aeiouAEIOU] { printf("%s is a vowel\n", yytext); }
[a-zA-Z]     { printf("%s is a consonant\n", yytext); }
.            { }
%%

int yywrap() { return 1; }

int main() {
    printf("Enter character to check:\n");
    yylex();
    return 0;
}
