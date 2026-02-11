%{
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
void yyerror(char *s);
int yylex();
%}

%union { float fval; }
%token <fval> NUMBER
%token ABS FABS COT
%type <fval> expr

%left '*'

%%
line: expr { printf("Result: %f\n", $1); } ;

expr: expr '*' expr { $$ = $1 * $3; }
    | ABS '(' expr ')' { $$ = fabs($3); }
    | FABS '(' expr ')' { $$ = fabs($3); }
    | COT '(' expr ')' { $$ = 1.0/tan($3); }
    | '(' expr ')' { $$ = $2; }
    | NUMBER { $$ = $1; }
    ;
%%

void yyerror(char *s) { printf("Error: %s\n", s); }
int main() { printf("Enter expression:\n"); yyparse(); return 0; }
