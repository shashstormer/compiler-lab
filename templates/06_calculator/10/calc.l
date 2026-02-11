%{
#include "calc.tab.h"
%}

%%
[0-9]+(\.[0-9]+)?  { yylval.fval = atof(yytext); return NUMBER; }
"log5"  { return LOG5; }
"log3"  { return LOG3; }
"sin"   { return SIN; }
"acos"  { return ACOS; }
[ \t]   ;
\n      return 0;
.       return yytext[0];
%%

int yywrap() { return 1; }
