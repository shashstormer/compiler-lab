%{
#include <stdio.h>
#include <math.h>
void yyerror(char *s);
int yylex();
%}

%union { float fval; }
%token <fval> NUMBER
%token LOG LOG10
%type <fval> expr

%left '-'
%left '/'

%%
line: expr { printf("Result: %f\n", $1); } ;

expr: expr '-' expr { $$ = $1 - $3; }
    | expr '/' expr { $$ = $1 / $3; }
    | LOG '(' expr ')' { $$ = log($3); }
    | LOG10 '(' expr ')' { $$ = log10($3); }
    | '(' expr ')' { $$ = $2; }
    | NUMBER { $$ = $1; }
    ;
%%

void yyerror(char *s) { printf("Error: %s\n", s); }
int main() { printf("Enter expression:\n"); yyparse(); return 0; }
