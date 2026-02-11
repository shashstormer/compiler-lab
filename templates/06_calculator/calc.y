%{
#include <stdio.h>
void yyerror(char *s);
int yylex();
%}

%token NUMBER

%left '+' '-'
%left '*' '/'

%%
expr: expr '+' expr { printf("Result: %d\n", $1 + $3); }
    | expr '-' expr { printf("Result: %d\n", $1 - $3); }
    | expr '*' expr { printf("Result: %d\n", $1 * $3); }
    | expr '/' expr { printf("Result: %d\n", $1 / $3); }
    | '(' expr ')'  { $$ = $2; }
    | NUMBER        { $$ = $1; }
    ;
%%

void yyerror(char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    printf("Enter expression:\n");
    yyparse();
    return 0;
}
