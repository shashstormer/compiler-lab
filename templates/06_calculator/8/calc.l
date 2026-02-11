%{
#include "calc.tab.h"
%}

%%
[0-9]+(\.[0-9]+)?  { yylval.fval = atof(yytext); return NUMBER; }
"ceil"  { return CEIL; }
"floor" { return FLOOR; }
[ \t]   ;
\n      return 0;
.       return yytext[0];
%%

int yywrap() { return 1; }
