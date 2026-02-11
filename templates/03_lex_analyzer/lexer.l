%{
#include <stdio.h>
%}

DIGIT       [0-9]
ID          [a-zA-Z][a-zA-Z0-9]*

%%
"int"|"float"|"char"|"void"|"return"    { printf("KEYWORD: %s\n", yytext); }
{DIGIT}+                                { printf("INTEGER: %s\n", yytext); }
{DIGIT}+"."{DIGIT}+                     { printf("FLOAT: %s\n", yytext); }
{ID}                                    { printf("IDENTIFIER: %s\n", yytext); }
"+"|"-"|"*"|"/"|"="                     { printf("OPERATOR: %s\n", yytext); }
"{"|"}"|"("|")"|";"|","                 { printf("SEPARATOR: %s\n", yytext); }
[ \t\n]                                 { ; }
.                                       { printf("UNKNOWN: %s\n", yytext); }
%%

int yywrap() {
    return 1;
}

int main(int argc, char **argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
    } else {
        printf("Usage: ./a.out <input_file>\n");
        printf("Enter input (Ctrl+D to end):\n");
    }
    yylex();
    return 0;
}
