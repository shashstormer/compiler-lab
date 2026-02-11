%{
#include "calc.tab.h"
%}

%%
[0-9]+(\.[0-9]+)?  { yylval.fval = atof(yytext); return NUMBER; }
"sin"   { return SIN; }
"cos"   { return COS; }
"asin"  { return ASIN; }
"acos"  { return ACOS; }
[ \t]   ;
\n      return 0;
.       return yytext[0];
%%

int yywrap() { return 1; }
