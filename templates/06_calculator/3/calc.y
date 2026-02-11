%{
#include <stdio.h>
#include <math.h>
void yyerror(char *s);
int yylex();
%}

%union { float fval; }
%token <fval> NUMBER
%token SINH COSH ASIN ACOS
%type <fval> expr

%left '+' '-'

%%
line: expr { printf("Result: %f\n", $1); } ;

expr: expr '+' expr { $$ = $1 + $3; }
    | expr '-' expr { $$ = $1 - $3; }
    | SINH '(' expr ')' { $$ = sinh($3); }
    | COSH '(' expr ')' { $$ = cosh($3); }
    | ASIN '(' expr ')' { $$ = asin($3); }
    | ACOS '(' expr ')' { $$ = acos($3); }
    | '(' expr ')' { $$ = $2; }
    | NUMBER { $$ = $1; }
    ;
%%

void yyerror(char *s) { printf("Error: %s\n", s); }
int main() { printf("Enter expression:\n"); yyparse(); return 0; }
