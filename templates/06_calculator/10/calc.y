%{
#include <stdio.h>
#include <math.h>
void yyerror(char *s);
int yylex();
%}

%union { float fval; }
%token <fval> NUMBER
%token LOG5 LOG3 SIN ACOS
%type <fval> expr

%%
line: expr { printf("Result: %f\n", $1); } ;

expr: LOG5 '(' expr ')' { $$ = log($3)/log(5); }
    | LOG3 '(' expr ')' { $$ = log($3)/log(3); }
    | SIN '(' expr ')' { $$ = sin($3); }
    | ACOS '(' expr ')' { $$ = acos($3); }
    | '(' expr ')' { $$ = $2; }
    | NUMBER { $$ = $1; }
    ;
%%

void yyerror(char *s) { printf("Error: %s\n", s); }
int main() { printf("Enter expression:\n"); yyparse(); return 0; }
