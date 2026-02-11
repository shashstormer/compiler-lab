%{
#include "calc.tab.h"
%}

%%
[0-9]+(\.[0-9]+)?  { yylval.fval = atof(yytext); return NUMBER; }
"tan"   { return TAN; }
"atan"  { return ATAN; }
"tanh"  { return TANH; }
[ \t]   ;
\n      return 0;
.       return yytext[0];
%%

int yywrap() { return 1; }
