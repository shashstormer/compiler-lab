%{
#include "calc.tab.h"
%}

%%
[0-9]+(\.[0-9]+)?  { yylval.fval = atof(yytext); return NUMBER; }
"log"   { return LOG; }
"log10" { return LOG10; }
"log2"  { return LOG2; }
"abs"   { return ABS; }
[ \t]   ;
\n      return 0;
.       return yytext[0];
%%

int yywrap() { return 1; }
