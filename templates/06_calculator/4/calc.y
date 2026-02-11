%{
#include <stdio.h>
#include <math.h>
void yyerror(char *s);
int yylex();
%}

%union { float fval; }
%token <fval> NUMBER
%token TAN ATAN TANH
%type <fval> expr

%left '*' '/'

%%
line: expr { printf("Result: %f\n", $1); } ;

expr: expr '*' expr { $$ = $1 * $3; }
    | expr '/' expr { $$ = $1 / $3; }
    | TAN '(' expr ')' { $$ = tan($3); }
    | ATAN '(' expr ')' { $$ = atan($3); }
    | TANH '(' expr ')' { $$ = tanh($3); }
    | '(' expr ')' { $$ = $2; }
    | NUMBER { $$ = $1; }
    ;
%%

void yyerror(char *s) { printf("Error: %s\n", s); }
int main() { printf("Enter expression:\n"); yyparse(); return 0; }
