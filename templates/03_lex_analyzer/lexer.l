%{
#include <math.h>
#include <stdio.h>
%}

DIGIT [0-9]
ID [a-z][a-z0-9]*

%%
{DIGIT}+ { printf( "An integer: %s (%d)\n", yytext, atoi( yytext ) ); }
{DIGIT}+"."{DIGIT}+ { printf( "A float: %s (%g)\n", yytext, atof( yytext ) ); }
if|then|begin|end|procedure|function|int|main|return|float|char { printf( "A keyword: %s\n", yytext ); }
{ID} { printf( "An identifier: %s\n", yytext ); }
"+"|"-"|"*"|"/" { printf( "An operator: %s\n", yytext ); }
","|";" { printf("special symbols: %s\n", yytext); }
[ \t\n]+ 
. { printf( "Unrecognized character: %s\n", yytext ); }
%%

int yywrap(){}

int main( int argc, char **argv )
{
    FILE *fp;
    char filename[50];
    printf("Enter the filename: \n");
    scanf("%s",filename);
    fp = fopen(filename,"r");
    yyin = fp;
    yylex();
    return 0;
}
