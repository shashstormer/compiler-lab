%{
#include <stdio.h>
#include <math.h>
void yyerror(char *s);
int yylex();
%}

%union { float fval; }
%token <fval> NUMBER
%token POW EXP
%type <fval> expr

%left '-'
%left '/'

%%
line: expr { printf("Result: %f\n", $1); } ;

expr: expr '-' expr { $$ = $1 - $3; }
    | expr '/' expr { $$ = $1 / $3; }
    | POW '(' expr ',' expr ')' { $$ = pow($3, $5); }
    | EXP '(' expr ')' { $$ = exp($3); }
    | '(' expr ')' { $$ = $2; }
    | NUMBER { $$ = $1; }
    ;
%%

void yyerror(char *s) { printf("Error: %s\n", s); }
int main() { printf("Enter expression:\n"); yyparse(); return 0; }
