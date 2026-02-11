%{
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
void yyerror(char *s);
int yylex();
%}

%union { float fval; }
%token <fval> NUMBER
%token LOG LOG10 LOG2 ABS
%type <fval> expr

%%
line: expr { printf("Result: %f\n", $1); } ;

expr: LOG '(' expr ')' { $$ = log($3); }
    | LOG10 '(' expr ')' { $$ = log10($3); }
    | LOG2 '(' expr ')' { $$ = log2($3); }
    | ABS '(' expr ')' { $$ = fabs($3); }
    | '(' expr ')' { $$ = $2; }
    | NUMBER { $$ = $1; }
    ;
%%

void yyerror(char *s) { printf("Error: %s\n", s); }
int main() { printf("Enter expression:\n"); yyparse(); return 0; }
